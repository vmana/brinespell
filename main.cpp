#include "soma.h"
#include "database.h"
#include "test/unit_test.h"
#include "thread/background.h"
#include "Wt/WServer.h"
#include "fuzzy.h"

unique_ptr<WApplication> create_application(const Wt::WEnvironment& env)
{
	auto app = make_unique<soma>(env);
	return app;
}

// based uppon Wt:WRun
int server_run(int argc, char *argv[], ApplicationCreator createApplication)
{
	background c_background;
	thread thread_background(&background::main, &c_background);
	try
	{
		WServer server(argv[0]);
		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

		server.addEntryPoint(Wt::EntryPointType::Application, createApplication, "", "/img/favicon.ico");
		if (server.start())
		{
			// wait for signal
			int sig = WServer::waitForShutdown();
			std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
			server.stop();
			c_background.started = false;
			pthread_cancel(thread_background.native_handle());
			thread_background.join();
		}
	}
	catch (WServer::Exception& e)
	{
		std::cerr << e.what() << "\n";
		c_background.started = false; thread_background.join();
		return 1;
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
		c_background.started = false; thread_background.join();
		return 1;
	}
	cout << "****    end server_run    ****" << endl;
	return 0;
}

int main(int argc, char *argv[])
{
	system::undefunct();

	// check for preprod
	for (int i = 1; i < argc; ++i)
	{
		string arg(argv[i]);

		if (arg == "--prod")
		{
			global::production = true;
		}
		else if (arg == "--preprod")
		{
			global::pre_production = true;
		}
		else if (arg == "--query")
		{
			global::show_queries = true;
		}
		else if (arg == "--remote")
		{
			global::remote_production = true;
		}
	}

	unit_test test;
	if (test.test_all()) debug_line("All unit tests done");

	{
		try
		{
			// try update, if existing db
			soma_database::update_database();
		} catch (dbo::Exception e) { debug_line(e.what()); exit(0); }
		try
		{
			soma_database D;
			soma_database::new_session();
			// try to create database. It will fail if the schema isn't empty
			if (global::try_database_creation) D.create_database();
			// try to create some default values
			D.create_default();
		} catch (dbo::Exception e) { debug_line(e.what()); exit(0); }
	}

	/* if (!file::exists(global::path_data)) system::mkdir(global::path_data); */

	return server_run(argc, argv, &create_application);
}

