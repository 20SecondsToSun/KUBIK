#include "KubikException.h"

const char* KubikException::what() const throw() 
{ 
	return mMessage; 
}

ExcBigFileSizeOfScreenSaver::ExcBigFileSizeOfScreenSaver() throw()
{
	sprintf( mMessage, "Большой размер файла у заставки (video <= 50мб | image <= 10мб)" );
	isCritical = true;
}

ExcConfigFileParsing::ExcConfigFileParsing() throw()
{
	sprintf( mMessage, "Ошибка в файле конфигурации" );
	isCritical = true;
}

ExcFileDoesNotExist::ExcFileDoesNotExist() throw()
{
	sprintf( mMessage, "Файла не существует" );
	isCritical = true;
}
