#pragma once

namespace kubik
{
	class Server
	{
	public:
		static Server& getInstance()
		{
			static Server server;
			return server;
		};	

		void login();
		void init();

		int getPhotoID() const;
		std::string getPhotoLink() const;
		void standInfo();
		void postPhoto(const std::string& path);
		//void sendToEmails(const std::string& path);
		void sendToEmails(int appID, int photoID, const std::string& emails);
		//void sendToEmailsThread(int appID, int photoID, const std::string& emails);

		boost::signals2::signal<void()> photoUploadSuccess;
		boost::signals2::signal<void()> photoUploadError;

	private:
		std::string access_token;
		std::string SERVER = "http://hypercubic.familyagency.ru";
		std::string user_name = "qwe@qwe.qwe";
		std::string password = "qwertY12+";
		std::string grant_type = "password";
		std::string client_id = "f3d259ddd3ed8ff3843839b";
		std::string client_secret = "4c7f6f8fa93d59c45502c0ae8c4a95b";

		ThreadRef mediaLoadThread;

		bool isLogin, threadLoad;
		int photo_id;
		std::string link;

		void threadLogin();
		void threadPostPhoto();
		void update();
		void sendToEmailsThread(int appID, int photoID, const std::string& emails);

		int errorCount;
		bool photoIsPosted;


		struct PhotoObj
		{
			std::string base64Str;
			int height;
		}
		photoObj;


	};

	inline Server& server()
	{
		return Server::getInstance();
	};
}