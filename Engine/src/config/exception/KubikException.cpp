#include "KubikException.h"

const char* KubikException::what() const throw() 
{ 
	return mMessage; 
}

ExcBigFileSizeOfScreenSaver::ExcBigFileSizeOfScreenSaver() throw()
{
	sprintf( mMessage, "Большой размер файла у заставки (video <= 50мб | image <= 10мб)");
	isCritical = true;
}

ExcConfigFileParsing::ExcConfigFileParsing() throw()
{
	sprintf(mMessage, "Ошибка в файле конфигурации");
	isCritical = true;
}

ExcFileDoesNotExist::ExcFileDoesNotExist() throw()
{
	sprintf( mMessage, "Файла не существует" );
	isCritical = true;
}

ExcGameDoesNotExist::ExcGameDoesNotExist() throw()
{
	sprintf( mMessage, "Игра не существует" );
	isCritical = true;
}

StickerDoesNotExist::StickerDoesNotExist() throw()
{
	sprintf( mMessage, "Стикера не существует" );
	isCritical = true;
}


