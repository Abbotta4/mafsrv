#include <sqlpp11/postgresql/connection.h>
#include <sqlpp11/select.h>
#include <sqlpp11/remove.h>
#include <sqlpp11/insert.h>
#include <sqlpp11/update.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <jwtpp/jwtpp.hh>
#include <users.h>
#include <pgdb.h>

sqlpp::postgresql::connection getDB() {
    auto config = std::make_shared<sqlpp::postgresql::connection_config>();
    config->host = "localhost";
    config->user = "postgres";
    config->password = "";
    config->dbname = "postgres";
    sqlpp::postgresql::connection db(config);
    return db;
}

std::string selectFromUsers(int uid) {
    mafsrv::PublicUsers users;
    auto db = getDB();
    auto res = db(select(all_of(users))
                 .from(users)
                 .where(users.uid == uid));
    if (!res.empty()) {
        rapidjson::Document d;
        rapidjson::Value &obj = d.SetObject();

        using gsr = rapidjson::GenericStringRef<char>;

        std::string name = res.front().username;
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

int insertIntoUsers(std::string body) {
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
                 .set(users.username = name,
                      users.gamesPlayed = 0,
                      users.gamesWon = 0,
                      users.joined = std::chrono::system_clock::now()));

    return 0;
}

int updateIntoUsers(int uid, std::string body) {
    rapidjson::Document dom;
    dom.Parse(body.c_str());

    mafsrv::PublicUsers users;

    auto db = getDB();
    auto s = dynamic_update(db, users).dynamic_set().dynamic_where(users.uid == uid);

    for (auto& m : dom.GetObject()) {
        if (m.name == "uid") {
            continue;
        } else if (m.name == "name") {
            s.assignments.add(users.username = m.value.GetString());
        } else if (m.name == "joined") {
            s.assignments.add(users.joined = std::chrono::time_point<std::chrono::system_clock>(std::chrono::seconds(m.value.GetInt())));
        } else if (m.name == "games played") {
            s.assignments.add(users.gamesPlayed = m.value.GetInt());
        } else if (m.name == "games won") {
            s.assignments.add(users.gamesWon = m.value.GetInt());
        } else {
            throw std::runtime_error("Invalid users key");
        }
    }

    auto res = db(s);

    return 0;
}

int insertIntoThreads(std::string body) {
    rapidjson::Document dom;
    dom.Parse(body.c_str());

    std::string name;
    rapidjson::Value::ConstMemberIterator it = dom.FindMember("name");
    if (it != dom.MemberEnd())
        name = it->value.GetString();
    else
        throw std::invalid_argument("malformed request");

    std::cout << "got past find name" << std::endl;
    mafsrv::PublicThreads threads;
    auto db = getDB();
    auto res = db(insert_into(threads)
                 .set(threads.name = name));
    std::cout << "got past db" << std::endl;
    return 0;
}

std::string selectAllThreads() {
    mafsrv::PublicThreads threads;
    auto db = getDB();
    auto res = db(select(all_of(threads))
                 .from(threads)
                 .unconditionally());
    if (!res.empty()) {
        rapidjson::Document d;
        rapidjson::Value &obj = d.SetObject();

        using gsr = rapidjson::GenericStringRef<char>;

        // fix this ugly garbage
        for (const auto& row : res) {
            std::string name = row.name;
            uint64_t tid = row.tid;
            std::string stid = std::to_string(tid);
            rapidjson::Value vname, vtid;
            vname.SetString(name.c_str(), name.size(), d.GetAllocator());
            vtid.SetString(stid.c_str(), stid.size(), d.GetAllocator());
            obj.AddMember(vtid, vname, d.GetAllocator());
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }

    return "";
}

std::string selectFromThreads(int tid) {
    mafsrv::PublicThreads threads;
    auto db = getDB();
    auto res = db(select(all_of(threads))
                 .from(threads)
                 .where(threads.tid == tid));
    if (!res.empty()) {
        rapidjson::Document d;
        rapidjson::Value &obj = d.SetObject();

        using gsr = rapidjson::GenericStringRef<char>;

        std::string name = res.front().name;
        obj.AddMember(gsr{"name"}, gsr{name.c_str()}, d.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }

    return "";
}

int updateIntoThreads(int tid, std::string body) {
    rapidjson::Document dom;
    dom.Parse(body.c_str());

    mafsrv::PublicThreads threads;

    auto db = getDB();
    auto s = dynamic_update(db, threads).dynamic_set().dynamic_where(threads.tid == tid);

    for (auto& m : dom.GetObject()) {
        if (m.name == "tid") {
            continue;
        } else if (m.name == "name") {
            s.assignments.add(threads.name = m.value.GetString());
        } else {
            throw std::runtime_error("Invalid threads key");
        }
    }

    auto res = db(s);

    return 0;
}

int deleteFromThreads(int tid) {
    mafsrv::PublicThreads threads;
    auto db = getDB();
    auto res = db(remove_from(threads)
                 .where(threads.tid == tid));

    return 0; // DELETE_OK
}

std::string sha256(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string tryLogin(std::string body) {
    mafsrv::PublicUsers users;
    rapidjson::Document dom;
    dom.Parse(body.c_str());

    // parse request body
    std::string usernameAttempt, passwordAttempt;
    rapidjson::Value::ConstMemberIterator it = dom.FindMember("username");
    if (it != dom.MemberEnd())
        usernameAttempt = it->value.GetString();
    else
        throw std::invalid_argument("malformed request");
    it = dom.FindMember("password");
    if (it != dom.MemberEnd())
        passwordAttempt = it->value.GetString();
    else
        throw std::invalid_argument("malformed request");

    // request hashed password and salt from db
    auto db = getDB();
    auto res = db(select(users.password, users.passwordSalt)
                 .from(users)
                 .where(users.username == usernameAttempt));

    // valid username
    if (!res.empty()) {
        std::string passwordSalt = res.front().passwordSalt;
        std::string saltedPassword = res.front().password;
        std::string saltedAttempt = sha256(passwordAttempt + passwordSalt);
        std::cout << saltedAttempt << std::endl;
        bool passwordMatches = saltedAttempt == saltedPassword;
        if (!passwordMatches) {
            rapidjson::Document d;
            rapidjson::Value &obj = d.SetObject();
            using gsr = rapidjson::GenericStringRef<char>;
            obj.AddMember(gsr{"status"}, gsr{"login failed"}, d.GetAllocator());
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            d.Accept(writer);
            throw std::runtime_error(buffer.GetString());
        }

        // fill out jwt "claims" (headers?): https://auth0.com/docs/secure/tokens/json-web-tokens/json-web-token-claims
        jwtpp::claims cl;
        cl.set().iss("jtl mafia");
        cl.set().sub(usernameAttempt);
        const auto now = std::chrono::system_clock::now();
        const Json::Int twentyOneDays = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() + 1814400;
        cl.set().any("exp", twentyOneDays);

        jwtpp::sp_crypto h512 = std::make_shared<jwtpp::hmac>("secret", jwtpp::alg_t::HS512);

        // sign and finish jwt
        std::string bearer = jwtpp::jws::sign_bearer(cl, h512);
        std::cout << bearer << std::endl;

        // store bearer in db
        mafsrv::PublicJwt jwt;
        auto res = db(insert_into(jwt)
                 .set(jwt.token = bearer,
                      jwt.authedUsername = usernameAttempt,
                      jwt.expiration = twentyOneDays));

        rapidjson::Document d;
        rapidjson::Value &obj = d.SetObject();

        using gsr = rapidjson::GenericStringRef<char>;

        obj.AddMember(gsr{"status"}, gsr{"ok"}, d.GetAllocator());
        obj.AddMember(gsr{"jwt"}, gsr{bearer.c_str()}, d.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }

    return "login failed";
}
