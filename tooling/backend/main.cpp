#include "server.hpp"
#include <cstdlib>

constexpr const char *SERVER_IP = "127.0.0.1";
constexpr const int SERVER_PORT = 8080;
std::string run_command;

struct TestPrototype : public msce::IPrototype
{
  double a;
  std::string b;
  std::vector<double> c;
  std::vector<std::string> d;

  MSCE_DEFINE_PROTOTYPE(TestPrototype, a, b, c, d)
};

MSCE_REGISTER_PROTOTYPE(TestPrototype, TestPrototype, a, b, c, d)

int main(int argc, char **argv)
{

  if (!argv || !argv[0])
  {
    logger.log_error("Failed to start server, no arguments provided when "
                     "expected at least 1(run command)");
    return 1;
  }
  else
    run_command = std::string(argv[0]);

  if (std::system("./build.sh") != 0)
  {
    logger.log_error("Failed to build the page.");
    return 1;
  }

  msce::init_engine();

  httplib::Server svr;
  svr.set_logger(
      [](const httplib::Request &req, const httplib::Response &res)
      {
        logger.log_info("{} {} -> {}", req.method.c_str(), req.path.c_str(),
                        res.status);
      });
  svr.set_error_handler(
      [](const httplib::Request &req, httplib::Response &res)
      {
        logger.log_info("error: {} {} -> {}", req.method.c_str(),
                        req.path.c_str(), res.status);
      });
  svr.set_exception_handler(
      [](const httplib::Request &req, httplib::Response &res,
         std::exception_ptr ep)
      {
        try
        {
          if (ep) std::rethrow_exception(ep);
        }
        catch (const std::exception &e)
        {
          logger.log_info("handler threw: {}", e.what());
        }
        res.status = 500;
      });
  std::filesystem::create_directories(".msce/prototypes");

  svr.set_mount_point("/assets", "./frontend/dist/assets");

  svr.Get("/", handle_main_page);
  svr.Get("/types", get_types);
  svr.Get("/component/whois", get_component_types);
  svr.Post("/type/fields", get_type_fields);
  svr.Post("/log", log_message);
  svr.Get("/prototype/whois", get_prototype_names);
  svr.Get("/prototype/ids", get_prototype_ids);
  svr.Post("/prototype/create", create_prototype);
  svr.Post("/prototype/edit", edit_prototype);
  svr.Post("/prototype/fields", get_prototype_fields);

  svr.Get("/entity/ids", get_entity_ids);
  svr.Post("/entity/create", create_entity);
  svr.Post("/entity/edit", edit_entity);
  svr.Post("/entity/fields", get_entity_fields);

  // SPA fallback for client-side routes
  svr.Get("/.*",
          [](const httplib::Request &req, httplib::Response &res)
          {
            if (req.path.rfind("/prototype/", 0) == 0 ||
                req.path.rfind("/type", 0) == 0 || req.path == "/types" ||
                req.path == "/log" || req.path.rfind("/assets", 0) == 0)
              return;
            res.set_file_content("./frontend/dist/index.html", "text/html");
          });

  std::system(
      std::format("xdg-open http://{}:{} &", SERVER_IP, SERVER_PORT).c_str());
  logger.log_info("Server starts listening at http://{}:{} from: {}", SERVER_IP,
                  SERVER_PORT, run_command);
  svr.listen(SERVER_IP, SERVER_PORT);
}