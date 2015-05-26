#pragma once
#include "services/social/SocShare.h"
#include "Socialsettings.h"

namespace kubik
{
	typedef std::shared_ptr<class Facebook> FacebookRef;

	class Facebook : public SocShare
	{
	public:
		Facebook();
	
		void		logOut() override;
		void		update() override;	
		void		signInUpdate();
		void		setPhotoAlbumName(const string &name);

	private:
		void		postText(const std::string& textStatus) override;
		void		postPhoto(const std::string& textStatus, const std::vector<std::string>& filesPath) override;
		void		facebookPostThread();
		void		waitLoadingComplete();
		void		postPhotosToFbAlbum(const std::string& textStatus, const std::vector<std::string>& filesPath);

		std::string facebookAlbumNameToPost;
		std::string facebookAlbumId;		
	};
}