#pragma once

#include "engine.core/sys.h"
#include "device_direct3d.h"

namespace render
{

class buffer
{
public:
    device_direct3d*	m_device;
    ID3D11Buffer*		m_buffer;
    size_t				m_size;
    int					m_use;
    bool				m_isDynamic;

public:

    buffer(device_direct3d* device) 
		: m_device(device)
		, m_size(0)
		, m_use(0) 
	{}
    
	~buffer()
	{
		if ( m_buffer )
			m_buffer->Release();
	}

    ID3D11Buffer*& GetBuffer()
    {
        return m_buffer;
    }

    virtual size_t GetSize() const
    {
        return m_size;
    }
    
	virtual void*  Map(size_t start, size_t size, int flags = 0)
	{
		UNREFERENCED(start);
		UNREFERENCED(size);

		D3D11_MAP mapFlags = D3D11_MAP_WRITE;
		if (flags & eMapFlags::Discard)    
			mapFlags = D3D11_MAP_WRITE_DISCARD;    
		if (flags & eMapFlags::Unsynchronized)    
			mapFlags = D3D11_MAP_WRITE_NO_OVERWRITE;    

		D3D11_MAPPED_SUBRESOURCE subresource;
		if ( SUCCEEDED(m_device->GetImmediateContext()->Map(m_buffer, 0, mapFlags, 0, &subresource) ) )
			return subresource.pData;        
		
		return nullptr;
	}

    virtual bool   Unmap(void *m)
	{
		UNREFERENCED(m);
		m_device->GetImmediateContext()->Unmap(m_buffer, 0);		
		return true;
	}

    virtual bool   Data(int use, const void* buffer, size_t size)
	{
		if (m_buffer && m_size >= size)
		{
			if (m_isDynamic)
			{
				if (!buffer)
					return true;

				void* v = Map(0, size, eMapFlags::Discard);
				if (v)
				{
					memcpy(v, buffer, size);
					Unmap(v);
					return true;
				}
			}
			else
			{
				m_device->GetImmediateContext()->UpdateSubresource(m_buffer, 0, NULL, buffer, 0, 0);
				return true;
			}
		}
		if (m_buffer)
		{
			m_buffer = nullptr;
			m_size = 0;
			m_use = 0;
			m_isDynamic = false;
		}

		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		if (use & eBufferUsage::ReadOnly)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.CPUAccessFlags = 0;
		}
		else
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_isDynamic = true;
		}

		switch(use & eBufferUsage::TypeMask)
		{
		case eBufferUsage::Vertex:  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
		case eBufferUsage::Index:   desc.BindFlags = D3D11_BIND_INDEX_BUFFER;  break;
		case eBufferUsage::Constant:
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			size += ((size + 15) & ~15) - size;
			break;
		}

		desc.ByteWidth = (unsigned)size;

		D3D11_SUBRESOURCE_DATA sr;
		sr.pSysMem = buffer;
		sr.SysMemPitch = 0;
		sr.SysMemSlicePitch = 0;

		HRESULT hr = m_device->GetDevice()->CreateBuffer(&desc, buffer ? &sr : nullptr, &m_buffer);
		if (SUCCEEDED(hr))
		{
			m_use = use;
			m_size = desc.ByteWidth;
			return 1;
		}
		return 0;
	}
};

} // render