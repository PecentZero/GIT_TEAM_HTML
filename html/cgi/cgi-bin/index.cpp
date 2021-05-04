#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include "cookie.h"

using namespace sql;
using namespace std;
using namespace cgicc;

void printHTMLHead();
void printHeaderMenuArea();
void printHomeBannerArea();
void printCitiesArea();
void printCitiesSubfunction(char* imgsrc, char* cityName);
void printEventsArea(char* city, int page, sql::Connection *con);
void printFooterArea();
void printOptionalJavascript();

void redirectToLogin();

void makeAdvertisement(char* author_id, char* dateTime, char* location, char* title, char* link);
void makeArticle(char* author_id, char* dateTime, char* location, char* views, char* imgsrc, char* title, char* text);

int main()
{
	// create cgicc instance for cookie
	Cgicc cgi;
	const_cookie_iterator cci;
	
	// create a mysql connection
	sql::Driver *driver;
	sql::Connection *con;
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "0812");
	con->setSchema("security_project");

	// get the cookie value
	char* cookie;
	const CgiEnvironment& env = cgi.getEnvironment();
	cci = env.getCookieList().begin();
	cookie = strdup((cci->getValue()).c_str());

	// check the session
	char* userID = session_check(cookie, con);

	if (!userID)
	{
		// if not logged in, redirect to the login page
		redirectToLogin();
		return 0;
	}

	// if already logged in
	
	// get the GET parameter for the city
	char* city;
	form_iterator fi = cgi.getElement("city");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		city = strdup((**fi).c_str());
	} else {
		// nothing specified for the city name
		city = "Seoul";
	}

	// get the GET parameter for the post page
	int p_page;
	fi = cgi.getElement("page");
	if ( !fi->isEmpty() && fi != (*cgi).end()){
		p_page = atoi(strdup((**fi).c_str()));
	} else {
		// nothing specified for the post page
		page = 1;
	}
	
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	printHTMLHead();
	cout << "<body>\n";
	printHeaderMenuArea();
	printHomeBannerArea();
	printCitiesArea();
	printEventsArea(city, page, con);
	printFooterArea();
	printOptionalJavascript();
	cout << "</body>\n";
	cout << "</html>\n";
}

void printHTMLHead()
{
	cout << "<head>\n";
    cout << "<!-- Required meta tags -->\n";
    cout << "<meta charset=\"utf-8\">\n";
    cout << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n";
    cout << "<link rel=\"icon\" href=\"img/favicon.png\" type=\"image/png\">\n";
    cout << "<title>Team HTML</title>\n";
    cout << "<!-- Bootstrap CSS -->\n";
    cout << "<link rel=\"stylesheet\" href=\"css/bootstrap.css\">\n";
    cout << "<link rel=\"stylesheet\" href=\"vendors/linericon/style.css\">\n";
    cout << "<link rel=\"stylesheet\" href=\"css/font-awesome.min.css\">\n";
    cout << "<link rel=\"stylesheet\" href=\"vendors/lightbox/simpleLightbox.css\">\n";
    cout << "<link rel=\"stylesheet\" href=\"vendors/nice-select/css/nice-select.css\">\n";
    cout << "<!-- main css -->\n";
    cout << "<link rel=\"stylesheet\" href=\"css/style.css\">\n";
    cout << "<link rel=\"stylesheet\" href=\"css/responsive.css\">\n";
    cout << "</head>\n";

	return;
}

void printHeaderMenuArea()
{
	cout << "<!--================ Start Header Menu Area =================-->\n";
    cout << "<header class=\"header_area\">\n";
    cout << "<div class=\"main_menu\">\n";
    cout << "<div class=\"container\">\n";
    cout << "<nav class=\"navbar navbar-expand-lg navbar-light\">\n";
    cout << "<div class=\"container\" style=\"height:60px\">\n";
    cout << "<a class=\"navbar-brand logo_h\" href=\"index.html\"><h3 style=\"color:white\">Team HTML<h3></a>\n";
    cout << "<button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarSupportedContent\" aria-controls=\"navbarSupportedContent\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">\n";
    cout << "<span class=\"icon-bar\"></span>\n";
    cout << "<span class=\"icon-bar\"></span>\n";
    cout << "<span class=\"icon-bar\"></span>\n";
    cout << "</button>\n";
    cout << "<div class=\"collapse navbar-collapse offset\" id=\"navbarSupportedContent\">\n";
    cout << "<ul class=\"nav navbar-nav menu_nav ml-auto\" style=\"height:80px\">\n";
    cout << "<li class=\"nav-item \"><a class=\"nav-link\" href=\"index.html\">Home</a></li>\n"; 
    cout << "<li class=\"nav-item\"><a class=\"nav-link\" href=\"profile.html\">My Profile</a></li>\n"; 
    cout << "<li class=\"nav-item\"><a class=\"nav-link\" href=\"settings.html\">Settings</a></li>\n"; 
    cout << "<li class=\"nav-item\"><a class=\"nav-link\" href=\"login.html\">Log Out</a>\n";
    cout << "</ul>\n";
    cout << "</div>\n"; 
    cout << "</div>\n";
    cout << "</nav>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</header>\n";

	return;
}

void printHomeBannerArea()
{
	cout << "<!--================ Home Banner Area =================-->\n";
    cout << "<section class=\"banner_area\">\n";
    cout << "<div class=\"banner_inner d-flex align-items-center\">\n";
    cout << "<div class=\"overlay bg-parallax\" data-stellar-ratio=\"0.9\" data-stellar-vertical-offset=\"0\" data-background=\"\"></div>\n";
    cout << "<div class=\"container\">\n";
    cout << "<div class=\"banner_content text-center single_sidebar_widget popular_post_widget\">\n";
    cout << "<h2>Great Time for Sharing</h2>]\n";
    cout << "<p>Let others know about your event!</p>\n";
    cout << "<span class=\"input-group-btn\"><br>\n";
    cout << "<a href= \"create_post.html\"><button class=\"btn btn-default\" type=\"button\">Create a post</button></a>\n";
    cout << "</span>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</section>\n";
    cout << "<!--================ End Home Banner Area =================-->\n";

	return;
}

void printCitiesArea()
{
	cout << "<!--================ Start of Cities Area =================-->\n";
	cout << "<section class=\"blog_categorie_area\">\n";
    cout << "<div class=\"container\">\n";
    cout << "<div class=\"row\">\n";
	printCitiesSubfunction("https://www.wapititravel.com/blog/wp-content/uploads/2019/07/Seoul_South_Korea.jpg.webp", "Seoul");
	printCitiesSubfunction("https://www.costacruises.com/content/dam/costa/inventory-assets/ports/PUS/pus-busan-port-1.jpg.image.750.563.low.jpg", "Busan");
	printCitiesSubfunction("https://www.lottehotelmagazine.com/resources/c90d48a5-ec3d-46c9-aacd-fae6011accc0_img_art_daejeon_detail01.jpg", "Daejeon");
	cout << "</div>\n";
    cout << "</div>\n";
    cout << "</section>\n";
    cout << "<!--================ End of Cities Area =================-->\n";

	return;
}

void printCitiesSubfunction(char* imgsrc, char* cityName)
{
	cout << "<div class=\"col-lg-4\">\n";
    cout << "<div class=\"categories_post\">\n";
    cout << "<img src=\"" << imgsrc << "\" alt=\"post\">\n";
    cout << "<div class=\"categories_details\">\n";
    cout << "<div class=\"categories_text\">\n";
    cout << "<a href=\"#\"><h5>" << cityName << "</h5></a>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
}

void printEventsArea(char* city, int p_page, int a_page, sql::Connection *con)
{
	cout << "<!--================ Events Area =================-->\n";
        cout << "<section class=\"blog_area\">\n";
        cout << "<div class=\"container\">\n";
        cout << "<div class=\"row\">\n";
        cout << "<div class=\"col-lg-8\">\n";
        cout << "<div class=\"blog_left_sidebar\">\n";
	
	sql::ResultSet *res_post;
	sql::ResultSet *res_adv;
	sql::ResultSet *res_post_count;
	sql::PreparedStatement *pstmt_post;
	sql::PreparedStatement *pstmt_adv;
	sql::PreparedStatement *pstmt_post_count;

	// determine how many posts there are total for the city
	int post_count;	
	string sql_post_count = "select count(*) from posts where location=?";
	pstmt_post_count = con->prepareStatement(sql_post_count);
	pstmt_post_count->setString(1, city);
	res_post_count = pstmt_post_count->executeQuery();
	if (res_post_count->next())
	{
		char* result = strdup((res_post_count->getString("count(*)")).c_str());
		delete res_post_count;
		delete pstmt_post_count;
		post_count = atoi(result);
	}
	else
	{
		delete res_post_count;
		delete pstmt_post_count;
		post_count = 0;
	}
	
	int total_num_post_page = post_count / 10 + 1;
	if (p_page > total_num_post_page) p_page = total_num_post_page;

	// get the post data from the DB
	string sql_post = "SELECT * FROM posts where location=? order by datetime DESC";
	pstmt_post = con->prepareStatement(sql_post);
	pstmt_post->setString(1, city);
	res_post = pstmt_post->executeQuery();

	for (int i = 0; i < (total_num_post_page - 1)*10; i++)
	{
		res_post->next();
	}
	int count = 0
	while (res_post_count->next() && count < 10)
	{
		char* author_id = strdup((res_post_count->getString("author_id")).c_str());
		char* dateTime = strdup((res_post_count->getDatetime("datetime")).c_str());
		char* location = strdup((res_post_count->getString("location")).c_str());
		char* views = strdup((res_post_count->getString("views")).c_str());
		char* imgsrc = strdup((res_post_count->getString("imgsrc")).c_str());
		char* title = strdup((res_post_count->getString("title")).c_str());
		char* text = strdup((res_post_count->getString("text")).c_str());
		makeArticle(author_id, dateTime, location, views, imgsrc, title, text);
		
		count++;
	}

	delete res_post;
	delete pstmt_post;

	// show the post page numbers
	post_pagination(city, p_page, a_page, total_num_post_page, total_num_adv_page);

	printUntilAdvertisementStart();
	
	// get the advertisement data from the DB in random order
	string sql_adv = "SELECT * FROM advertisements where location=? order by rand()";
	pstmt_adv = con->prepareStatement(sql_adv);
	pstmt_adv->setString(1, city);
	res_adv = pstmt_adv->executeQuery();
	
	count = 0;

	// print maximum 10 advertisements
	while (res_adv_count->next() && count < 10)
	{
		char* author_id = strdup((res_adv_count->getString("author_id")).c_str());
		char* dateTime = strdup((res_adv_count->getDatetime("datetime")).c_str());
		char* location = strdup((res_adv_count->getDatetime("location")).c_str());
		char* title = strdup((res_adv_count->getString("title")).c_str());
		char* link = strdup((res_adv_count->getString("link")).c_str());
		
		makeAdvertisement(title, imgsrc, link);

		count++;
	}

	// print the ending html tags of the event_section
	endEventSection(city, p_page, a_page, total_num_post_page, total_num_adv_page);

	delete res_adv;
	delete pstmt_adv;

	return;
}

void printUntilAdvertisementStart()
{
	cout << "</div>\n";
	cout << "</div>\n";
	cout << "<div class=\"col-lg-4\">\n";
    cout << "<div class=\"blog_right_sidebar\">\n";
    cout << "<aside class=\"single_sidebar_widget search_widget\">\n";
    cout << "<div class=\"input-group\">\n";
    cout << "<input type=\"text\" class=\"form-control\" placeholder=\"Search Posts\">\n";
    cout << "<span class=\"input-group-btn\">\n";
    cout << "<button class=\"btn btn-default\" type=\"button\"><i class=\"lnr lnr-magnifier\"></i></button>\n";
    cout << "</span>\n";
    cout << "</div><input-group>\n";
    cout << "<div class=\"br\"></div>\n";
    cout << "</aside>\n";
    cout << "<aside class=\"single_sidebar_widget popular_post_widget\">\n";
	return;
}

void makeAdvertisement(char* title, char* imgsrc, char* link)
{
	cout << "<div class=\"media post_item\">\n";
    cout << "<img src=\"" << imgsrc << "\" alt=\"post\">\n";
    cout << "<div class=\"media-body\">\n";
    cout << "<h3>" << title << "</h3>\n";
    cout << "<a href=\"" << link << "\"><p>link</p></a>\n";
    cout << "</div>\n";
    cout << "</div>\n";

	return;
}

void makeArticle(char* author_id, char* dateTime, char* location, char* likes, char* imgsrc, char* title, char* text)
{
	// make profile link for the author_id
	char* profile_link;

	cout << "<article class=\"row blog_item\">\n";
    cout << "<div class=\"col-md-3\">\n";
    cout << "<div class=\"blog_info text-right\">\n";
    cout << "<div class=\"post_tag\">\n";
    cout << "<a >" << location << "</a>\n";
    cout << "</div>\n";
    cout << "<ul class=\"blog_meta list\">\n";
    cout << "<li><a href=\"" << profile_link << "\">" << authorID "<i class=\"lnr lnr-user\"></i></a></li>\n";
    cout << "<li></li><li><a>" << dateTime << "<i class=\"lnr lnr-calendar-full\"></i></a></li>\n";
    cout << "<li><a>" << likes << "likes<i class=\"lnr lnr-eye\"></i></a></li>\n";
    cout << "</ul>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "<div class=\"col-md-9\">\n";
    cout << "<div class=\"blog_post\">\n";
    cout << "<img src=\"" << imgsrc << "\" alt=\"\">\n";
    cout << "<div class=\"blog_details\">\n";
    cout << "<a href=\"#\"><h2>" << title << "</h2></a>\n";
    cout << "<p>" << text << "</p>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</article>\n";

	return;
}

void endEventSection()
{
	cout << "<div class=\"br\"></div>\n";
    cout << "</aside>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</section>\n";
    cout << "<!--================Events Area =================-->\n";

	return;
}

void post_pagination(char* city, int p_page, int a_page, int total_num_post_page, int total_num_adv_page)
{
	int prev_page = (p_page==1)?1:p_page-1;
	int next_page = (p_page==total_num_post_page)?total_num_post_page:p_page+1;

	cout << "<nav class=\"blog-pagination justify-content-center d-flex\">\n";
	cout << "<ul class=\"pagination\">\n";
	cout << "<li class=\"page-item\">\n";
	cout << "<a href=\"?city=" << city << "&page=" << prev_page << "\" class=\"page-link\" aria-label=\"Previous\">\n";
	cout << "<span aria-hidden=\"true\">\n";
	cout << "<span class=\"lnr lnr-chevron-left\"></span>\n";
	cout << "</span>\n";
	cout << "</a>\n";
	cout << "</li>\n";
	for (int i = 0; i < total_num_post_page; i++)
	{
		if (i + 1 == p_page)
		{
			cout << "<li class=\"page-item active\"><a href=\"?city=" << city << "&page=" << (i+1) << "\" class=\"page-link\">" << (i+1) << "</a></li>\n";
		}
		else
		{
			cout << "<li class=\"page-item\"><a href=\"?city=" << city << "&page=" << (i+1) << "\" class=\"page-link\">" << (i+1) << "</a></li>\n";
		}
	}
	cout << "<li class=\"page-item\">\n";
	cout << "<a href=\"?city=" << city << "&page=" << next_page << "\" class=\"page-link\" aria-label=\"Next\">\n";
	cout << "<span aria-hidden=\"true\">\n";
	cout << "<span class=\"lnr lnr-chevron-right\"></span>\n";
	cout << "</span>\n";
	cout << "</a>\n";
	cout << "</li>\n";
	cout << "</ul>\n";
	cout << "</nav>\n";

	return;
}

void printFooterArea()
{
	cout << "<!--================ Start footer Area  =================-->\n";	
    cout << "<footer>\n";
	cout << "<div class=\"footer-bottom\">\n";
    cout << "<div class=\"container\">\n";
    cout << "<div class=\"row d-flex\">\n";
    cout << "<p class=\"col-lg-12 footer-text text-center\">\n";
    cout << "Copyright &copy;<script>document.write(new Date().getFullYear());</script> All rights reserved |Done by <b>Team HTML</b>\n";
	cout << "</p>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</div>\n";
    cout << "</footer>\n";
    cout << "<!--================ End footer Area  =================-->\n";
 
	return;
}

void printOptionalJavascript()
{
	cout << "<!-- Optional JavaScript -->\n";
    cout << "<!-- jQuery first, then Popper.js, then Bootstrap JS -->\n";
    cout << "<script src=\"js/jquery-3.2.1.min.js\"></script>";
    cout << "<script src=\"js/popper.js\"></script>\n";
    cout << "<script src=\"js/bootstrap.min.js\"></script>\n";
    cout << "<script src=\"js/stellar.js\"></script>\n";
    cout << "<script src=\"vendors/lightbox/simpleLightbox.min.js\"></script>\n";
    cout << "<script src=\"vendors/nice-select/js/jquery.nice-select.min.js\"></script>\n";
    cout << "<script src=\"js/jquery.ajaxchimp.min.js\"></script>\n";
    cout << "<script src=\"js/mail-script.js\"></script>\n";
    cout << "<script src=\"js/theme.js\"></script>\n";

	return;
}

void redirectToLogin()
{
	cout << "Content-type:text/html\r\n\r\n";
	cout << "<!doctype html>\n";
	cout << "<html lang=\"en\">\n";
	cout << "<body>\n";
	cout << "<script> window.location.href = \"../index.html\"; </script>\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return;
}

