#include <drogon/drogon.h>
#include <iostream>
#include <drogon/orm/DbClient.h>
#include <string>

void notice_view(std::shared_ptr<drogon::orm::DbClient> drogon_backend)
{
	drogon::app().registerHandler(
		"/api/notice-view",
		[drogon_backend](const drogon::HttpRequestPtr& req,
			std::function<void (const drogon::HttpResponsePtr &)> &&callback)
		{
			auto idStr = req->getParameter("id");
			int idInt = 0;
			try {
				idInt = std::stoi(idStr);
			} catch (...) {
				idInt = 0;
			}
	
			if(idStr.empty())
			{
				auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", "id가 필요합니다"}});
				callback(resp);
				return;
			}
			drogon_backend->execSqlAsync(
				"SELECT title, contents FROM write WHERE id = $1",
				[callback](const drogon::orm::Result &r)
				{
					/*
					if(r.size() == 0)
					{
						auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error","게시글 없음"}});
						callback(resp);
						return;
					}
					*/
					Json::Value json;
					json["title"] = r[0]["title"].as<std::string>();
					json["content"] = r[0]["contents"].as<std::string>();
					auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
					callback(resp);
				},
				[callback](const drogon::orm::DrogonDbException &e)
				{
					auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", e.base().what()}});
					callback(resp);
				},
				idInt
			);
		},
		{drogon::Get}
	);
}
	
	



void notice_board(std::shared_ptr<drogon::orm::DbClient> drogon_backend)
{
	drogon::app().registerHandler
	(
		"/api/notice-board",
		[drogon_backend](const drogon::HttpRequestPtr& req,
		std::function<void (const drogon::HttpResponsePtr &)> &&callback)
		{
			auto resp = drogon::HttpResponse::newHttpResponse();
			resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
			*drogon_backend << "SELECT id, title, contents, created_at FROM write"
				>> [resp, callback](const drogon::orm::Result &r) {
					Json::Value jsonResp;
					Json::Value postsArray;
					if (r.size() > 0) {
						for (const auto &row : r) {
							Json::Value post;
							post["id"] = row["id"].as<int>();
							post["title"] = row["title"].as<std::string>();
							post["contents"] = row["contents"].as<std::string>();
							post["created_at"] = row["created_at"].as<std::string>();
							postsArray.append(post);
						}
						jsonResp["status"] = "success";
						jsonResp["posts"] = postsArray;
						jsonResp["count"] = (int)r.size();
					} else {
						jsonResp["status"] = "success";
						jsonResp["posts"] = Json::arrayValue;
						jsonResp["count"] = 0;
						jsonResp["message"] = "No posts found";
					}
					resp->setBody(jsonResp.toStyledString());
					callback(resp);
				}
				>> [resp, callback](const drogon::orm::DrogonDbException &e) {
					Json::Value jsonResp;
					jsonResp["status"] = "error";
					jsonResp["message"] = "Database error: " + std::string(e.base().what());
					resp->setBody(jsonResp.toStyledString());
					callback(resp);
				};
			},
			{drogon::Get}
	);

}

void signup(std::shared_ptr<drogon::orm::DbClient> drogon_backend)
{
	drogon::app().registerHandler
	(
		"/Account/signup/submit",  // 요청 경로
		[drogon_backend](const drogon::HttpRequestPtr& req,
		std::function<void (const drogon::HttpResponsePtr &)> &&callback)
		{
			// body
			auto params = req->getParameters();
			std::string id = params["id"];
			std::string password = params["password"];
			std::string name = params["name"];
			std::string nickname = params["nickname"];
			std::string phonenumber = params["phonenumber"];
			std::string address = params["address"];
			std::string email = params["email"];

			*drogon_backend << "INSERT INTO member (id, password, name, nickname, phonenumber, address, email)"
				"VALUES ($1, $2, $3, $4, $5, $6, $7)" << id << password << name << nickname << phonenumber << address << email
				>> [callback](const drogon::orm::Result &r)
				{
					auto resp = drogon::HttpResponse::newRedirectionResponse("/success.html");
					callback(resp);
				}
				>> [callback](const drogon::orm::DrogonDbException &e)
				{
					auto resp = drogon::HttpResponse::newHttpResponse();
					resp->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
					callback(resp);
				};
		},

		{drogon::Post}
	);
}

void login(std::shared_ptr<drogon::orm::DbClient> drogon_backend)
{
	drogon::app().registerHandler
	(
		"/Account/login/submit",
		[drogon_backend](const drogon::HttpRequestPtr& req,
		std::function<void (const drogon::HttpResponsePtr &)> &&callback)
		{
			auto params = req->getParameters();
			std::string id = params["id"];
			std::string password = params["password"];
			 
			std::string sql = "SELECT nickname FROM member WHERE id = '" + id + "' AND password = '" + password + "'";
			drogon_backend->execSqlAsync
			(
				sql,
				[drogon_backend, callback, id, req](const drogon::orm::Result &r)
				{
					auto resp = drogon::HttpResponse::newHttpResponse();
					if (r.size() == 0)
					{
						resp->setBody("User not found.");
					}
					else
					{
						auto session = req->getSession();
						session->insert("user_id", id);
						resp->setBody("Login success!");
					}
					callback(resp);
				},
				[callback](const drogon::orm::DrogonDbException &e)
				{
					auto resp = drogon::HttpResponse::newHttpResponse();
					resp->setBody(std::string("DB Error: ") + e.base().what());
					callback(resp);
				}
				
			);
		},
		{drogon::Post}		
	);
}

void write(std::shared_ptr<drogon::orm::DbClient> drogon_backend)
{
	drogon::app().registerHandler
	(
		"/board/write/submit",
		[drogon_backend](const drogon::HttpRequestPtr& req,
		std::function<void (const drogon::HttpResponsePtr &)> &&callback)
		{
			auto params = req->getParameters();
			std::string title = params["title"];
			std::string contents = params["contents"];
			*drogon_backend << "INSERT INTO write (title, contents)"
			"VALUES ($1, $2)" << title << contents
			>> [callback](const drogon::orm::Result &r)
			{
				auto resp = drogon::HttpResponse::newRedirectionResponse("/writesuccess.html");
				callback(resp);
			}
			>> [callback](const drogon::orm::DrogonDbException &e)
			{
				auto resp = drogon::HttpResponse::newHttpResponse();
				resp->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
				callback(resp);
			};
		},
		{drogon::Post}
	);
}
	
int main() 
{
	// Drogon init
	drogon::app().addListener("127.0.0.1",8080);
	drogon::app().setDocumentRoot("/server/web/peta/client");
	drogon::app().setThreadNum(0);
	drogon::app().enableSession();
	

	//Postgresql connect
	std::string connInfo =
		"host=/server/web/peta/database "
		"user=phantom "
		"password=akskqkqh "
		"dbname=peta_member_info";


	auto drogon_backend = drogon::orm::DbClient::newPgClient(connInfo, 1, false);

	drogon_backend->execSqlAsync
	(
		"SELECT 1",
		[](const drogon::orm::Result &r)
		{
			LOG_DEBUG << "DB 연결됨";
		},
		[](const drogon::orm::DrogonDbException &e)
		{
			LOG_ERROR << "DB 연결실패: " << e.base().what();
		}
	);

	signup(drogon_backend);
	login(drogon_backend);
	write(drogon_backend);
	notice_board(drogon_backend);
	notice_view(drogon_backend);
	std::cout << "로그인 회원가입 설정 완료" << std::endl;	
	drogon::app().registerHandler(
		"/Account/sessionCheck",
		[](const drogon::HttpRequestPtr &req,
			std::function<void(const drogon::HttpResponsePtr &)> &&callback)
		{
			Json::Value jsonObj;
	
			// 세션 포인터 가져오기
			auto session = req->session(); // getSession() 대신 session() 사용 권장
	
			if (!session)
			{
				jsonObj["loggedIn"] = false;
			}
			else
			{
				// 세션에 user_id가 있는지 확인
				try
				{
					auto user_id_opt = session->getOptional<std::string>("user_id");
					if (user_id_opt)
					{
						jsonObj["loggedIn"] = true;
						jsonObj["user_id"] = *user_id_opt;
					}
					else
					{
						jsonObj["loggedIn"] = false;
					}
				}
				catch (const std::exception &e)
				{
					LOG_ERROR << "세션 접근 오류; " << e.what();
					jsonObj["loggedIn"] = false;
				}
			}	
			auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonObj);
			callback(resp);
		},
		{drogon::Get}
	);
	drogon::app().run();	
	return 0;
}
