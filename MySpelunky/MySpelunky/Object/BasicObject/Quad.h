#pragma once
class Quad
{
protected:
	Quad() {}

public:
	Quad(wstring path);
	Quad(Vector2 size, wstring path);
	Quad(Vector2 size, wstring path, Vector2 scale);
	virtual ~Quad();

	virtual void Update();
	virtual void Render();

	virtual void CreateVertices();
	void CreateVertices(Vector2 scale);
	virtual void CreateData(wstring path);

	Vector2 GetImageSize() { return _srv.lock()->GetImageSize(); }
	vector<Vertex_Texture>& GetVertices() { return _vertices; }

	void SetPS(shared_ptr<PixelShader> ps) { _ps = ps; }
	void SetPV(shared_ptr<VertexShader> vs) { _vs = vs; }

protected:
	vector<Vertex_Texture> _vertices;
	vector<UINT> _indices;

	Vector2 _size;

	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	weak_ptr<VertexShader> _vs;
	weak_ptr<PixelShader> _ps;

	weak_ptr<SRV> _srv;
};

