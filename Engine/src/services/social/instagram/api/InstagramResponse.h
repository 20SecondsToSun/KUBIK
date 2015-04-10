#pragma once
#include "cinder/app/AppNative.h"
#include "instagram/api/Pagination.h"
#include "instagram/api/Metadata.h"

namespace instagram
{   
	template<class T> class InstagramResponse
	{	
		Pagination pagination;
		Metadata meta;
		std::list<T> data;

	public:
		// getters
		Pagination getPagination()
		{
			return pagination;
		}

		Metadata getMetadata()
		{
			return meta;
		}

		const std::list<T>& getData()
		{
			return data;
		}

		// setters
		void setPagination(Pagination value)
		{
			pagination = value;
		}

		void setMetadata(Metadata meta)
		{
			meta = meta;
		}

		void setData(T data)
		{
			data = data;
		}

		void clear()
		{
			data.clear();
		}

		void parse(const std::string& json)
		{
			clear();
			console() << json<<endl;
			try
			{
				meta.parse(JsonTree(json).getChild("meta"));

				if (meta.okCode())
				{
					try
					{
						pagination.parse(JsonTree(json).getChild("pagination"));
					}
					catch (...)
					{

					}

					JsonTree jdata = JsonTree(json).getChild("data");

					for (auto _data : jdata)
					{
						T object;
						object.parse(_data);
						data.push_back(object);
					}
				}
				else
				{
					ci::app::console() << "code:  " << meta.getCode() << std::endl;
				}
			}
			catch (...)
			{

			}
		}
	};
}