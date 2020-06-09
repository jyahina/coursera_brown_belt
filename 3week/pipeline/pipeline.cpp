#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
	string from;
	string to;
	string body;
};


class Worker {
public:
	virtual ~Worker() = default;
	virtual void Process(unique_ptr<Email> email) = 0;
	virtual void Run() {
		// только первому worker-у в пайплайне нужно это имплементировать
		throw logic_error("Unimplemented");
	}

protected:
	// реализации должны вызывать PassOn, чтобы передать объект дальше
	// по цепочке обработчиков
	void PassOn(unique_ptr<Email> email) const
	{
		if (worker)
			worker->Process(move(email));
	};

public:
	void SetNext(unique_ptr<Worker> next)
	{
		worker = move(next);
	};

private:
	unique_ptr<Worker> worker;
};


class Reader : public Worker {
public:
	Reader(istream& input_)
		: input(input_)
	{
	};
	
	void Run() override
	{
		string from, to, body;

		while (getline(input, from) && getline(input, to) && getline(input, body))
		{
			auto email = make_unique<Email>();

			email->from = from;
			email->to = to;
			email->body = body;

			Process(move(email));
		}
	}

	void Process(unique_ptr<Email> email) override
	{
		PassOn(move(email));
	};

private:
	istream& input;
};


class Filter : public Worker {
public:
	using Function = function<bool(const Email&)>;
	Filter(Function func)
		: f(func)
	{};

	void Process(unique_ptr<Email> email) override
	{
		if (f(*email))
			PassOn(move(email));
	};
	
private:
	Function f;
};


class Copier : public Worker {
public:
	Copier(string recipient)
		:address(recipient)
	{};

	void Process(unique_ptr<Email> email) override
	{
		unique_ptr<Email> newEmail;

		if (address != email->to)
		{
			newEmail = make_unique<Email>();

			newEmail->from = email->from;
			newEmail->to = address;
			newEmail->body = email->body;

		}

		PassOn(move(email));
		if (newEmail) PassOn(move(newEmail));
	};

private:
	string address;
};


class Sender : public Worker {
public:
	Sender(ostream& out)
		: output(out)
	{};

	void Process(unique_ptr<Email> email) override
	{
		output << email->from << endl;
		output << email->to << endl;
		output << email->body << endl;

		PassOn(move(email));
	};

private:
	ostream& output;
};


// реализуйте класс
class PipelineBuilder {
public:
	// добавляет в качестве первого обработчика Reader
	explicit PipelineBuilder(istream& in)
		: worker(make_unique<Reader>(in))
	{
		last = worker.get();
	};

	// добавляет новый обработчик Filter
	PipelineBuilder& FilterBy(Filter::Function filter)
	{
		Next(make_unique<Filter>(filter));

		return *this;
	};

	// добавляет новый обработчик Copier
	PipelineBuilder& CopyTo(string recipient)
	{
		Next(make_unique<Copier>(recipient));

		return *this;
	};

	// добавляет новый обработчик Sender
	PipelineBuilder& Send(ostream& out)
	{
		Next(make_unique<Sender>(out));

		return *this;
	};

	// возвращает готовую цепочку обработчиков
	unique_ptr<Worker> Build()
	{
		return move(worker);
	};

private:
	unique_ptr<Worker> worker;
	Worker* last;

	void Next(unique_ptr<Worker> w)
	{
		auto tmp = w.get();
		last->SetNext(move(w));
		last = tmp;
	}
};


void TestSanity() {
	string input = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"ralph@example.com\n"
		"erich@example.com\n"
		"I do not make mistakes of that kind\n"
		);
	istringstream inStream(input);
	ostringstream outStream;

	PipelineBuilder builder(inStream);
	builder.FilterBy([](const Email& email) {
		return email.from == "erich@example.com";
		});
	builder.CopyTo("richard@example.com");
	builder.Send(outStream);
	auto pipeline = builder.Build();

	pipeline->Run();

	string expectedOutput = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"erich@example.com\n"
		"richard@example.com\n"
		"Are you sure you pressed the right button?\n"
		);

	ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSanity);
	return 0;
}
