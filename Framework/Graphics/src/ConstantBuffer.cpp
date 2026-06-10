#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace TRengine;
using namespace TRengine::Graphics;

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mConstantBuffer == nullptr, "ConstantBuffer: terminate must be called");
}

void ConstantBuffer::Initialize(uint32_t bufferSize)
{
	auto device = GraphicsSystem::Get()->GetDevice();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "ConstantBuffer: failed to create buffer");
}
void ConstantBuffer::Terminate()
{
	SafeRelease(mConstantBuffer);
}

void ConstantBuffer::Update(const void* data) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetConstantBuffers(slot, 1, &mConstantBuffer);
}
void ConstantBuffer::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetConstantBuffers(slot, 1, &mConstantBuffer);
}