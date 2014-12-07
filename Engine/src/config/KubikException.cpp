#include "KubikException.h"

const char* KubikException::what() const throw() 
{ 
	return mMessage; 
}

ExcBigFileSizeOfScreenSaver::ExcBigFileSizeOfScreenSaver() throw()
{
	sprintf( mMessage, "������� ������ ����� � �������� (video <= 50�� | image <= 10��)" );
	isCritical = true;
}

ExcConfigFileParsing::ExcConfigFileParsing() throw()
{
	sprintf( mMessage, "������ � ����� ������������" );
	isCritical = true;
}

ExcFileDoesNotExist::ExcFileDoesNotExist() throw()
{
	sprintf( mMessage, "����� �� ����������" );
	isCritical = true;
}
