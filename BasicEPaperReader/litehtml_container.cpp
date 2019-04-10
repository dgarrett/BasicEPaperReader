#include <Arduino.h>

#include <litehtml.h>
#include "litehtml_container.h"

litehtml::uint_ptr container::create_font(const litehtml::tchar_t *faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics *fm)
{
	Serial.println("create_font");
	return new int;
}

void container::delete_font(litehtml::uint_ptr hFont)
{
	Serial.println("delete_font");
	delete hFont;
}

int container::text_width(const litehtml::tchar_t *text, litehtml::uint_ptr hFont)
{
	Serial.print("text_width: ");
	Serial.println(text);
	return 10;
}

void container::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t *text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position &pos)
{
	Serial.print("draw_text: ");
	Serial.println(text);
}

int container::pt_to_px(int pt)
{
	Serial.println("pt_to_px");
	return 1;
}

int container::get_default_font_size() const
{
	Serial.println("get_default_font_size");
	return 8;
}

const litehtml::tchar_t *container::get_default_font_name() const
{
	Serial.println("get_default_font_name");
	return "Test";
}

void container::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker &marker)
{
	Serial.println("draw_list_marker");
}

void container::load_image(const litehtml::tchar_t *src, const litehtml::tchar_t *baseurl, bool redraw_on_ready)
{
	Serial.println("load_image");
}

void container::get_image_size(const litehtml::tchar_t *src, const litehtml::tchar_t *baseurl, litehtml::size &sz)
{
	Serial.println("get_image_size");
}

void container::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint &bg)
{
	Serial.println("draw_background");
}

void container::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders &borders, const litehtml::position &draw_pos, bool root)
{
	Serial.println("draw_borders");
}

void container::set_caption(const litehtml::tchar_t *caption)
{
	Serial.println("set_caption");
}

void container::set_base_url(const litehtml::tchar_t *base_url)
{
	Serial.println("set_base_url");
}

void container::link(const std::shared_ptr<litehtml::document> &doc, const litehtml::element::ptr &el)
{
	Serial.println("link");
}

void container::on_anchor_click(const litehtml::tchar_t *url, const litehtml::element::ptr &el)
{
	Serial.println("on_anchor_click");
}

void container::set_cursor(const litehtml::tchar_t *cursor)
{
	Serial.println("set_cursor");
}

void container::transform_text(litehtml::tstring &text, litehtml::text_transform tt)
{
	Serial.println("transform_text");
}

void container::import_css(litehtml::tstring &text, const litehtml::tstring &url, litehtml::tstring &baseurl)
{
	Serial.println("import_css");
}

void container::set_clip(const litehtml::position &pos, const litehtml::border_radiuses &bdr_radius, bool valid_x, bool valid_y)
{
	Serial.println("set_clip");
}

void container::del_clip()
{
	Serial.println("del_clip");
}

void container::get_client_rect(litehtml::position &client) const
{
	Serial.println("get_client_rect");
	client.x = 0;
	client.y = 0;
	client.width = 400;
	client.height = 300;
}

std::shared_ptr<litehtml::element> container::create_element(const litehtml::tchar_t *tag_name,
												  const litehtml::string_map &attributes,
												  const std::shared_ptr<litehtml::document> &doc)
{
	Serial.print("create_element: ");
	Serial.println(tag_name);
	return nullptr;//std::make_shared<litehtml::element>();
}

void container::get_media_features(litehtml::media_features &media) const
{
	Serial.println("get_media_features");
}

void container::get_language(litehtml::tstring &language, litehtml::tstring &culture) const
{
	Serial.println("get_language");
}
//litehtml::tstring resolve_color(const litehtml::tstring &color) const { return litehtml::tstring(); }