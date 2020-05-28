'use strict';


/****    images    ****/

function w_image(id, src)
{
	// console.log('init ' + id);
	var img = document.getElementById(id);
	var bar = img.getElementsByClassName('widget_image_bar')[0];
	var content = img.getElementsByClassName('widget_image_content')[0];
	var view_mode = img.getElementsByClassName('widget_image_view_mode')[0];

	var border_top = img.getElementsByClassName('widget_image_border_top')[0];
	var border_left = img.getElementsByClassName('widget_image_border_left')[0];
	var border_right = img.getElementsByClassName('widget_image_border_right')[0];
	var border_bottom = img.getElementsByClassName('widget_image_border_bottom')[0];
	var corner_nw = img.getElementsByClassName('widget_image_corner_nw')[0];
	var corner_ne = img.getElementsByClassName('widget_image_corner_ne')[0];
	var corner_sw = img.getElementsByClassName('widget_image_corner_sw')[0];
	var corner_se = img.getElementsByClassName('widget_image_corner_se')[0];

	var mode = 'contain';
	var position = []; // init mouse position when moving / resizing
	var resize_class = '';
	var orig_h = 0, orig_w = 0, max_wh; // origin size, and maximum between width and height ('w' or 'h')
	var min_w_size = 60; // min width size
	var min_h_size = 30; // min height size

	content.style.backgroundImage = "url('" + src + "') ";
	content.style.backgroundRepeat = 'no-repeat';
	content.style.backgroundSize = 'contain';

	bar.onmousedown = on_bar_mousedown;

	function on_bar_mousedown(e)
	{
		e = e || window.event;
		e.preventDefault();
		if (e.buttons == 1) // left click
		{
			// get the mouse cursor position at startup
			position = [e.clientX, e.clientY];
			// img.style.opacity = 0.7;
			document.onmouseup = on_bar_mouseup;
			// call a function whenever the cursor moves
			document.onmousemove = on_bar_mousemove;
			// remove any animations
			img.classList.remove("widget_image_animated");
		}
	}

	function on_bar_mouseup(e)
	{
		// stop moving when mouse button is released
		// img.style.opacity = 1;
		document.onmouseup = null;
		document.onmousemove = null;
		Wt.emit(id, 'signal_move', img.offsetTop, img.offsetLeft);
	}

	function on_bar_mousemove(e)
	{
		e = e || window.event;
		e.preventDefault();
		// calculate the new cursor position:
		var delta_x = e.clientX - position[0];
		var delta_y = e.clientY - position[1];

		var new_position = [e.clientX, e.clientY];
		var new_top = img.offsetTop + delta_y;
		var new_left = img.offsetLeft + delta_x;

		if (img.offsetTop + delta_y < 0)
		{
			new_top = 0;
			new_position[1] = position[1];
		}
		if (img.offsetTop + img.offsetHeight + delta_y > window.innerHeight)
		{
			new_top = window.innerHeight - img.offsetHeight;
			new_position[1] = position[1]; // old position
		}
		if (img.offsetLeft + delta_x < 0)
		{
			new_left = 0;
			new_position[0] = position[0]; // old position
		}
		if (img.offsetLeft + img.offsetWidth + delta_x > window.innerWidth)
		{
			new_left = window.innerWidth - img.offsetWidth;
			new_position[0] = position[0];
		}

		position = new_position;

		img.style.top = new_top + "px";
		img.style.left = new_left + "px";
	}

	border_top.onmousedown = on_border_mousedown;
	border_left.onmousedown = on_border_mousedown;
	border_right.onmousedown = on_border_mousedown;
	border_bottom.onmousedown = on_border_mousedown;
	corner_nw.onmousedown = on_border_mousedown;
	corner_ne.onmousedown = on_border_mousedown;
	corner_sw.onmousedown = on_border_mousedown;
	corner_se.onmousedown = on_border_mousedown;

	function on_border_mousedown(e)
	{
		e = e || window.event;
		e.preventDefault();
		// isolate resize_class
		var main_class = '';
		for (i = 0; i < e.target.classList.length; i++)
		{
			var name = e.target.classList[i];
			if (name == 'widget_image_border' || name == 'widget_image_corner') continue;
			resize_class = name;
			break;
		}
		if (e.buttons == 1) // left click
		{
			// get the mouse cursor position at startup
			position = [e.clientX, e.clientY];
			document.onmouseup = on_border_mouseup;
			// call a function whenever the cursor moves
			document.onmousemove = on_border_mousemove;
			// remove any animations
			img.classList.remove("widget_image_animated");
		}
	}

	function on_border_mouseup(e)
	{
		// stop moving when mouse button is released
		document.onmouseup = null;
		document.onmousemove = null;

		if (content.style.backgroundSize == 'contain') autocrop_contain();

		Wt.emit(id, 'signal_resize', img.offsetTop, img.offsetLeft, img.offsetWidth, img.offsetHeight);
	}

	function on_border_mousemove(e)
	{
		e = e || window.event;
		e.preventDefault();
		// calculate the new cursor position:
		var delta_x = e.clientX - position[0];
		var delta_y = e.clientY - position[1];

		// boundaries checks
		var safe_margin = 8;

		var new_position = [e.clientX, e.clientY];

		/****    resize functions    ****/

		function resize_top()
		{
			var pos_bottom = img.offsetHeight + img.offsetTop;
			var new_top = img.offsetTop + delta_y;
			// prevent window overflow
			if (new_top < 0)
			{
				new_top = 0;
				new_position[1] = 0;
			}
			// prevent negative size
			else if (pos_bottom - new_top <= min_h_size)
			{
				new_top = pos_bottom - min_h_size; // old position
				new_position[1] = position[1];
			}
			var new_height = pos_bottom - new_top;
			// console.log(new_height + ' = ' + pos_bottom + ' - ' + new_top);
			img.style.top = new_top + 'px';
			img.style.height = new_height + 'px';
		}

		function resize_bottom()
		{
			var pos_top = img.offsetTop;
			var new_bottom = img.offsetTop + img.offsetHeight + delta_y;
			// prevent window overflow
			if (new_bottom > window.innerHeight)
			{
				new_bottom = window.innerHeight;
				new_position[1] = window.innerHeight;
			}
			// prevent negative size
			else if (new_bottom - pos_top <= min_h_size)
			{
				new_bottom = pos_top + min_h_size; // old position
				new_position[1] = new_bottom;
			}
			var new_height = new_bottom - pos_top;
			// console.log(new_height + ' = ' + new_bottom + ' - ' + pos_top);
			img.style.height = new_height + 'px';
		}

		function resize_left()
		{
			var pos_right = img.offsetLeft + img.offsetWidth;
			var new_left = img.offsetLeft + delta_x;
			// prevent window overflow
			if (new_left < 0)
			{
				new_left = 0;
				new_position[0] = 0;
			}
			// prevent negative size
			else if (pos_right - new_left <= min_w_size)
			{
				new_left = pos_right - min_w_size; // old position
				new_position[0] = position[0];
			}
			var new_width = pos_right - new_left;
			// console.log(new_height + ' = ' + pos_bottom + ' - ' + new_top);
			img.style.left = new_left + 'px';
			img.style.width = new_width + 'px';
		}

		function resize_right()
		{
			var pos_left = img.offsetLeft;
			var new_right = img.offsetLeft + img.offsetWidth + delta_x;
			// prevent window overflow
			if (new_right > window.innerWidth)
			{
				new_right = window.innerWidth;
				new_position[0] = window.innerWidth;
			}
			// prevent negative size
			else if (new_right - pos_left <= min_w_size)
			{
				new_right = pos_left + min_w_size; // old position
				new_position[0] = new_right;
			}
			var new_width = new_right - pos_left;
			// console.log(new_height + ' = ' + new_bottom + ' - ' + pos_top);
			img.style.width = new_width + 'px';
		}

		/****    actual resize call    ****/

		switch (resize_class)
		{
			case 'widget_image_border_top':
				resize_top();
				break;
			case 'widget_image_border_bottom':
				resize_bottom();
				break;
			case 'widget_image_border_left':
				resize_left();
				break;
			case 'widget_image_border_right':
				resize_right();
				break;
			case 'widget_image_corner_nw':
				resize_top();
				resize_left();
				break;
			case 'widget_image_corner_ne':
				resize_top();
				resize_right();
				break;
			case 'widget_image_corner_sw':
				resize_bottom();
				resize_left();
				break;
			case 'widget_image_corner_se':
				resize_bottom();
				resize_right();
				break;
		}

		position = new_position;

		if (mode == 'zoom') update_zoom_content();
	}

	function autocrop_contain()
	{
		// crop leftover when image is in contain mode
		if (orig_h == 0 || orig_w == 0) return; // should never happen
		var orig_ratio = orig_w / orig_h;
		var current_ratio = img.offsetWidth / img.offsetHeight;

		if (orig_ratio < current_ratio)
		{
			// reduce width
			var new_width = Math.ceil(img.offsetHeight * orig_w / orig_h);
			if (new_width < min_w_size) new_width = min_w_size;
			img.style.width = new_width + 'px';
		}
		else
		{
			// reduce height
			var new_height = Math.ceil(img.offsetWidth * orig_h / orig_w);
			if (new_height < min_h_size) new_height = min_h_size;
			img.style.height = new_height + 'px';
		}

	}

	// try to determine a good ratio for initial display
	var max_init_size = 500;
	var image = new Image();
	image.src = src;
	image.onload = function()
	{
		orig_w = image.naturalWidth;
		orig_h = image.naturalHeight;
		if (orig_h == 0 || orig_w == 0) return; // should never happen

		// set img width and height
		var max_size;
		var new_w = orig_w, new_h = orig_h;
		if (orig_w > orig_h)
		{
			max_wh = 'w';
			max_size = orig_w;
		}
		else
		{
			max_wh = 'h';
			max_size = orig_h;
		}

		// resize image if too large
		if (max_size > max_init_size)
		{
			if (max_wh == 'w')
			{
				// resize image
				new_w = max_init_size;
				new_h = Math.ceil(orig_h * max_init_size / orig_w);
			}
			else
			{
				// resize image
				new_h = max_init_size;
				new_w = Math.ceil(orig_w * max_init_size / orig_h);
			}
		}

		// set image size
		img.style.width = new_w + 'px';
		img.style.height = new_h + 'px';
	};

	/****    zoom & mouse drag functions    ****/

	var previous_zoom_e;
	var zoom_step = 0.1;
	var zoom_w = img.offsetWidth;
	var zoom_h = img.offsetHeight;
	var zoom_x = 0;
	var zoom_y = 0;

	content.addEventListener('wheel', on_content_wheel);
	content.addEventListener('mousedown', on_content_mousedown);

	function init_zoom_content()
	{
		zoom_w = orig_w;
		zoom_h = orig_h;
		zoom_x = 0;
		zoom_y = 0;

		content.style.backgroundSize = zoom_w + 'px '+ zoom_h + 'px';
		content.style.backgroundPosition = zoom_x + 'px '+ zoom_y + 'px';
	}

	function update_zoom_content()
	{
		if (orig_h == 0 || orig_w == 0) return; // should never happen

		// adjust values if img size has changed, which creates empty spaces
		if (zoom_w < img.offsetWidth)
		{
			zoom_w = img.offsetWidth;
			// update zoom_h to be in ratio
			zoom_h = Math.ceil(zoom_w * orig_h / orig_w);
		}
		else if (zoom_h < img.offsetHeight)
		{
			zoom_h = img.offsetHeight;
			// update zoom_w to be in ratio
			zoom_w = Math.ceil(zoom_h * orig_w / orig_h);
		}

		// update x, y
		if (zoom_x > 0)
		{
			zoom_x = 0;
		}
		else if (zoom_x < img.offsetWidth - zoom_w)
		{
			zoom_x = img.offsetWidth - zoom_w;
		}

		if (zoom_y > 0)
		{
			zoom_y = 0;
		}
		else if (zoom_y < img.offsetHeight - zoom_h)
		{
			zoom_y = img.offsetHeight - zoom_h;
		}

		// only use integers
		zoom_w = Math.round(zoom_w);
		zoom_h = Math.round(zoom_h);
		zoom_x = Math.round(zoom_x);
		zoom_y = Math.round(zoom_y);

		content.style.backgroundSize = zoom_w + 'px ' + zoom_h + 'px';
		content.style.backgroundPosition = zoom_x + 'px ' + zoom_y + 'px';
	}

	function on_content_wheel(e)
	{
		e = e || window.event;
		e.preventDefault();
		if (mode != 'zoom') return;
		img.classList.remove("widget_image_animated");

		var delta_y = 0;
		if (e.deltaY)
		{
			// FireFox 17+ (IE9+, Chrome 31+?)
			delta_y = e.deltaY;
		}
		else if (e.wheelDelta)
		{
			delta_y = -e.wheelDelta;
		}

		// as far as I know, there is no good cross-browser way to get the cursor position relative to the event target
		// we have to calculate the target element's position relative to the document, and subtrack that from the
		// cursor's position relative to the document
		var rect = img.getBoundingClientRect();
		var offset_x = e.pageX - rect.left - window.pageXOffset;
		var offset_y = e.pageY - rect.top - window.pageYOffset;

		// record the offset between the bg edge and cursor:
		var img_cursor_x = offset_x - zoom_x;
		var img_cursor_y = offset_y - zoom_y;

		// use the previous offset to get the percent offset between the bg edge and cursor
		var img_ratio_x = img_cursor_x / zoom_w;
		var img_ratio_y = img_cursor_y / zoom_h;

		// update the bg size
		if (delta_y < 0)
		{
			zoom_w += zoom_w * zoom_step;
			zoom_h += zoom_h * zoom_step;
		} else
		{
			zoom_w -= zoom_w * zoom_step;
			zoom_h -= zoom_h * zoom_step;
		}

		// take the percent offset and apply it to the new size
		zoom_x = offset_x - (zoom_w * img_ratio_x);
		zoom_y = offset_y - (zoom_h * img_ratio_y);

		update_zoom_content();

		// wt callback
		Wt.emit(id, 'signal_zoom', zoom_w, zoom_h, zoom_x, zoom_y);
	}

	function on_content_mousedown(e)
	{
		e = e || window.event;
		e.preventDefault();
		if (mode != 'zoom') return;

		previous_zoom_e = e;

		if (e.buttons == 1) // left click
		{
			document.onmouseup = on_content_mouseup;
			// call a function whenever the cursor moves
			document.onmousemove = on_content_mousemove;
			// remove any animations
			img.classList.remove("widget_image_animated");
		}
	}

	function on_content_mouseup(e)
	{
		document.onmouseup = null;
		document.onmousemove = null;

		// wt callback
		Wt.emit(id, 'signal_zoom', zoom_w, zoom_h, zoom_x, zoom_y);
	}

	// variable used when wt force zoom value
	img.wt_force_zoom = function(zoom_values)
	{
		var array_zoom = zoom_values.split(';');
		zoom_w = array_zoom[0];
		zoom_h = array_zoom[1];
		zoom_x = array_zoom[2];
		zoom_y = array_zoom[3];
		update_zoom_content();
	};

	function on_content_mousemove(e)
	{
		e = e || window.event;
		e.preventDefault();
		zoom_x += (e.pageX - previous_zoom_e.pageX);
		zoom_y += (e.pageY - previous_zoom_e.pageY);
		previous_zoom_e = e;
		update_zoom_content();
	}

	// variable used when wt change view mode
	img.wt_switch_view = function(new_mode)
	{
		mode = new_mode;
		if (mode == 'contain')
		{
			view_mode.className = 'widget_image_view_mode widget_image_view_contain';
			content.style.backgroundSize = 'contain';
			content.style.backgroundPosition = '0px 0px';
			autocrop_contain();
		}
		else if (mode == 'zoom')
		{
			view_mode.className = 'widget_image_view_mode widget_image_view_zoom';
			init_zoom_content();
		}
	};

}

function init_widget_image(id, src)
{
	w_image(id, src);
}

function wt_image_force_zoom(id, zoom_values)
{
	// zoom_values = 'zoom_w;zoom_h;zoom_x;zoom_y'
	var img = document.getElementById(id);
	img.wt_force_zoom(zoom_values);
}

function wt_image_switch_view(id, mode)
{
	var img = document.getElementById(id);
	img.wt_switch_view(mode);
}

/****    chat    ****/

function init_chat_box(wt_chat_id)
{
	var chat = document.getElementById(wt_chat_id);
	// change scrollbar display
	SimpleScrollbar.initEl(chat);

	const callback = function()
	{
		var scroll_id = chat.getElementsByClassName('ss-content');
		// console.log(scroll_id[0].scrollTop + ':' + scroll_id[0].scrollHeight);
		scroll_id[0].scrollTop = scroll_id[0].scrollHeight;
	};
	var observer = new MutationObserver(callback);
	observer.observe(chat, { attributes: false, childList: true, subtree: true });
}

/****    custom buttons    ****/

function init_selector_buttons()
{
	var radius_middle = 70; // middle round button
	var space_middle = 8;
	var moon_arc = 20;
	var button_width = 160;
	var button_height = 50;
	var button_font = 26;
	var text_color = '#f9e9b8';
	var bg_color = '#0a141c';

	// button left
	var canvas = document.getElementById('canvas_decoration_left');
	var context = canvas.getContext('2d');
	canvas.classList.add('selector_decoration');
	canvas.width = button_width + radius_middle;
	canvas.height = 140;
	canvas.style.left = 'calc(50% - ' + button_width + 'px - ' + radius_middle + 'px)';
	// canvas.style.bottom = 'calc(' + radius_middle + 'px - (' + canvas.height + 'px / 2))';
	canvas.style.bottom = '0px';

	context.fillStyle = bg_color;
	context.beginPath();
	// left circle
	context.arc((button_height / 2), (canvas.height / 2), (button_height / 2), 0, 2 * Math.PI, false);
	// rectangle button
	context.fillRect(button_height / 2, canvas.height / 2 - button_height / 2, button_width, button_height);
	// moon arc on the right
	context.arc(button_width + radius_middle - moon_arc, (canvas.height / 2), radius_middle, 0, 2 * Math.PI, false);
	context.fill();

	// remove half a circle of the right
	context.save();
	context.globalCompositeOperation = 'destination-out';
	context.beginPath();

	context.arc(button_width + radius_middle, (canvas.height / 2), radius_middle + space_middle, 0, 2 * Math.PI, false);
	context.fill();
	context.restore();

	// button left text
	context.font = button_font + 'px sans-serif';
	context.textAlign = "center";
	context.textBaseline = "middle";
	context.fillStyle = text_color;
	context.fillText('SECRET', button_width/2, canvas.height/2 + 3);

	// button right
	canvas = document.getElementById('canvas_decoration_right');
	context = canvas.getContext('2d');
	canvas.classList.add('selector_decoration');
	canvas.width = button_width + radius_middle;
	canvas.height = 140;
	canvas.style.right = 'calc(50% - ' + button_width + 'px - ' + radius_middle + 'px)';
	canvas.style.bottom = '0px';

	context.fillStyle =  bg_color;
	context.beginPath();
	// right circle
	context.arc(canvas.width - (button_height / 2), (canvas.height / 2), (button_height / 2), 0, 2 * Math.PI, false);
	// rectangle button
	context.fillRect(radius_middle, canvas.height / 2 - button_height / 2, button_width - (button_height / 2), button_height);
	// moon arc on the left
	context.arc(moon_arc, (canvas.height / 2), radius_middle, 0, 2 * Math.PI, false);
	context.fill();
	// remove half a circle of the left
	context.save();
	context.globalCompositeOperation = 'destination-out';
	context.beginPath();

	context.arc(0, (canvas.height / 2), radius_middle + space_middle, 0, 2 * Math.PI, false);
	context.fill();
	context.restore();

	// button right text
	context.font = button_font + 'px sans-serif';
	context.textAlign = "center";
	context.textBaseline = "middle";
	context.fillStyle = text_color;
	context.fillText('NORMAL', radius_middle + button_width/2, canvas.height/2 + 3);
}
