#ifndef widget_dynamic_H
#define widget_dynamic_H

#include "wcontainer.h"
#include "widget_image.h"
#include "token/widget_token.h"

class widget_dynamic : public wcontainer
{
	public:
		WContainerWidget *images;
		WContainerWidget *tokens;

		widget_dynamic();

		/****    images    ****/
		string open_image(string filename); // return image id
		static WContainerWidget* instance_images();
		static WContainerWidget* instance_tokens();
		static void open_shared_image(string filename, string id);
		static void move_image(string id, int top, int left);
		static void resize_image(string id, int top, int left, int width, int height);
		static void zoom_image(string id, int zoom_w, int zoom_h, int zoom_x, int zoom_y);
		static void switch_mode_image(string id, string mode);
		static void change_image_visibility(string id, bool visible);
		static void close_image(string id);
		static widget_image* search_image(string id);

		/****    images    ****/
		string open_token(string filename, int x, int y); // return token id
		static void open_shared_token(string filename, string id);
		static void move_token(string id, int top, int left);
		static void change_token_visibility(string id, bool visible);
		static void close_token(string id);
		static widget_token* search_token(string id);
};

#endif // widget_dynamic_H

