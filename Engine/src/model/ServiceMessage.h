#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;

class ServiceMessage
{
	public:
	ServiceMessage(int code)
	{
		this->code  = code;
	}

	ServiceMessage()
	{
		this->code  = -1;
	}

	string getMessage()
	{
		string text;
		switch (code)
		{
			case 100:
				text = "������ � ����� ������������";
			break;

			case 101:
				text = "����� �� ����������";
			break;

			default:
			break;
		}
		return text;
	}

private:
	int code;
};