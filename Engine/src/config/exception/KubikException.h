#pragma once

class KubikException : public cinder::Exception
{
public:
	const char* what() const throw();
protected:
	char							mMessage[2048];
	bool							isCritical;
	//friend class					
};

class ExcBigFileSizeOfScreenSaver : public KubikException 
{
public:
	ExcBigFileSizeOfScreenSaver() throw();
};

class ExcConfigFileParsing : public KubikException 
{
public:
	ExcConfigFileParsing() throw();
};

class ExcFileDoesNotExist : public KubikException 
{
public:
	ExcFileDoesNotExist() throw();
};

class ExcGameDoesNotExist : public KubikException 
{
public:
	ExcGameDoesNotExist() throw();
};

class StickerDoesNotExist : public KubikException 
{
public:
	StickerDoesNotExist() throw();
};


