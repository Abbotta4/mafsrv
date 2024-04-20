#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/select.h>
#include <sqlpp11/remove.h>
#include <sqlpp11/insert.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "users.h"
#include "pgdb.h"

sqlpp::postgresql::connection getDB() {
    auto config = std::make_shared<sqlpp::postgresql::connection_config>();
    config->host = "localhost";
    config->user = "postgres";
    config->password = "";
    config->dbname = "postgres";
    sqlpp::postgresql::connection db(config);
    return db;
}

std::string getFromUsers(int uid) {
    mafsrv::PublicUsers users;
    auto db = getDB();
    auto res = db(select(all_of(users))
		  .from(users)
		  .where(users.uid == uid));
    if (!res.empty()) {
	rapidjson::Document d;
	rapidjson::Value &obj = d.SetObject();

	using gsr = rapidjson::GenericStringRef<char>;

	std::string name = res.front().name;
	obj.AddMember(gsr{"name"}, gsr{name.c_str()}, d.GetAllocator());
	const auto joined = std::chrono::system_clock::time_point{res.front().joined.value()};
	obj.AddMember(gsr{"joined"}, std::chrono::duration_cast<std::chrono::seconds>(joined.time_since_epoch()).count(), d.GetAllocator());
	obj.AddMember(gsr{"games played"}, res.front().gamesPlayed, d.GetAllocator());
	obj.AddMember(gsr{"games won"}, res.front().gamesWon, d.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	return buffer.GetString();
    }

    return "";
}

int deleteFromUsers(int uid) {
    mafsrv::PublicUsers users;
    auto db = getDB();
    auto res = db(remove_from(users)
		  .where(users.uid == uid));

    return 0; // DELETE_OK
}

int insertUsers(std::string body) {
    rapidjson::Document dom;
    dom.Parse(body.c_str());

    std::string name, email;
    rapidjson::Value::ConstMemberIterator it = dom.FindMember("name");
    if (it != dom.MemberEnd())
	name = it->value.GetString();
    else
	throw std::invalid_argument("malformed request");
    it = dom.FindMember("email");
    if (it != dom.MemberEnd())
	email = it->value.GetString();
    else
	throw std::invalid_argument("malformed request");

    mafsrv::PublicUsers users;
    auto db = getDB();
    auto res = db(insert_into(users)
		  .set(users.name = name,
		       users.gamesPlayed = 0,
		       users.gamesWon = 0,
		       users.joined = std::chrono::system_clock::now()));

    return 0;
}
