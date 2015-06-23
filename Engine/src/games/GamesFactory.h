#pragma once
#include "IGame.h"
#include "ISettings.h"

namespace kubik
{
	template <class base>
	class GamesFactory
	{
	public:
		typedef std::shared_ptr<base> base_ptr;

		template <class derived>
		void reg(game::id const& name, std::shared_ptr<ISettings> setting)
		{
			console() << "reg game id :::::::::  " << name << endl;
			games[name] = base_type_ptr(new derived_type<derived>);
			settings[name] = setting;
		}

		base_ptr create(game::id const& name)
		{
			console() << "create game id :::::::::  " << name << endl;
			return games[name]->create(settings[name]);
		}

	private:
		class base_type
		{
		public:
			virtual ~base_type() {}
			virtual base_ptr create(std::shared_ptr<ISettings> set) = 0;
		};

		typedef std::shared_ptr<base_type> base_type_ptr;

		template <class T>
		class derived_type : public base_type
		{
		public:
			virtual base_ptr create(std::shared_ptr<ISettings> set)
			{
				return base_ptr(new T(set));
			}
		};

		typedef std::map<game::id, base_type_ptr> games_container;
		typedef std::map<game::id, std::shared_ptr<ISettings>> settings_container;

		games_container		games;
		settings_container  settings;
	};
}