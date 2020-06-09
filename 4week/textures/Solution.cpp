#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`
class Shape : public IShape
{
public:
	// Возвращает точную копию фигуры.
	// Если фигура содержит текстуру, то созданная копия содержит ту же самую
	// текстуру. Фигура и её копия совместно владеют этой текстурой.
	std::unique_ptr<IShape> Clone() const override
	{
		auto rectangle = MakeShape(shape_type);
		rectangle->SetPosition(position);
		rectangle->SetSize(size);
		
		if (texture)
			rectangle->SetTexture(texture);

		return rectangle;
	};

	void SetPosition(Point point) override
	{
		position = point;
	};

	Point GetPosition() const override
	{
		return position;
	};

	void SetSize(Size sizeShape) override
	{
		size = sizeShape;
	};

	Size GetSize() const override
	{
		return size;
	};

	void SetTexture(std::shared_ptr<ITexture> textureShape) override
	{
		texture = textureShape;
	};

	ITexture* GetTexture() const
	{
		return texture.get();
	};

	// Рисует фигуру на указанном изображении
	void Draw(Image&) const override
	{};

protected:
	char defaultPixel = '.';
	Point position = { 0, 0 };
	Size size = { 0, 0 };
	std::shared_ptr<ITexture> texture = nullptr;
	ShapeType shape_type;

	template<typename F>
	void DrawShape(Image& image, F& IsPointInShape) const
	{
		for (int Y = 0; Y < size.height && getY(Y) < image.size(); ++Y)
		{
			for (int X = 0; X < size.width && getX(X) < image[getY(Y)].size(); ++X)
			{
				if (!IsPointInShape(X, Y)) continue;

				image[getY(Y)][getX(X)] = (IsInTexture(X, Y))
					? texture->GetImage()[Y][X]
					: defaultPixel;
			}
		}
	};

	bool IsInTexture(int X, int Y) const
	{
		return (texture
			&& X < texture->GetSize().width
			&& Y < texture->GetSize().height);
	};

	int getX(int X) const
	{
		return position.x + X;
	};

	int getY(int Y) const
	{
		return position.y + Y;
	};
};

class RectangleShape : public Shape
{
public:

	void Draw(Image& image) const override
	{
		auto func = [&](int X, int Y) {return true; };
		DrawShape(image, func);
	};

private:
	ShapeType shape_type = ShapeType::Rectangle;
};

class EllipseShape : public Shape
{
public:
	void Draw(Image& image) const override
	{
		auto func = [&](int X, int Y) {return IsPointInEllipse({ X, Y }, size); };
		DrawShape(image, func);
	};

private:
	ShapeType shape_type = ShapeType::Ellipse;
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type)
{
	if (shape_type == ShapeType::Rectangle)
		return make_unique<RectangleShape>();
	else if (shape_type == ShapeType::Ellipse)
		return make_unique<EllipseShape>();

	return nullptr;
}