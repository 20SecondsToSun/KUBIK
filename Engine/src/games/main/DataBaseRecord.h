#pragma once

#include "Utils.h"

namespace kubik
{
	struct DataBaseRecord
	{
		int FilterNum;
		int PrintCardTemplate;
		//int PrintCardNum;
		//int PrintStickerNum;
		int CameraShotsNum;
		int CameraGoodShotsNum;
		bool FBShare;
		bool VKhare;
		bool TWShare;
		bool EmailShare;
		std::string Emails;
		std::string FBLink;
		std::string VKLink;
		std::string TWLink;

		const std::string delimeter;

		DataBaseRecord() : delimeter(";")
		{
			clear();
		};

		void clear()
		{
			FilterNum =
			PrintCardTemplate =
			//PrintStickerNum =
			CameraShotsNum =
			CameraGoodShotsNum = 0;

			FBShare =
			VKhare =
			TWShare =
			EmailShare = false;

			Emails =
			FBLink =
			VKLink =
			TWLink = "";
		}

		std::string getRecordTitle() const
		{
			using namespace std;
			return "Date" + delimeter +
				"FilterNum" + delimeter +
				"PrintCardTemplate" + delimeter +
				"CameraShotsNum" + delimeter +
				"CameraGoodShotsNum" + delimeter +
				"FBShare" + delimeter +
				"VKhare" + delimeter +
				"TWShare" + delimeter +
				"Emails" + delimeter +
				"FBLink" + delimeter +
				"VKLink" + delimeter +
				"TWLink";
		}

		std::string getRecord() const
		{
			using namespace std;

			auto timeStruct = Utils::getCurrentTime();
			auto date = Utils::fix2(to_string(timeStruct.tm_hour)) + ":"
				+ Utils::fix2(to_string(timeStruct.tm_min)) + ":"
				+ Utils::fix2(to_string(timeStruct.tm_sec));

			return date + delimeter +
				to_string(FilterNum) + delimeter +
				to_string(PrintCardTemplate) + delimeter +
				to_string(CameraShotsNum) + delimeter +
				to_string(CameraGoodShotsNum) + delimeter +
				to_string(FBShare) + delimeter +
				to_string(VKhare) + delimeter +
				to_string(TWShare) + delimeter +
				to_string(EmailShare) + delimeter +
				Emails + delimeter +
				FBLink + delimeter +
				VKLink + delimeter +
				TWLink + delimeter;
		}

		void save()
		{
			logger().log("==========================================save photobooth data record===================================================");
		}
	};
}