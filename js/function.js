'use strict';


/****    images    ****/

function w_image(id, src)
{
	// console.log('init ' + id);
	var img = document.getElementById(id);
	var bar = img.getElementsByClassName('widget_image_bar')[0];
	var content = img.getElementsByClassName('widget_image_content')[0];

	var border_top = img.getElementsByClassName('widget_image_border_top')[0];
	var border_left = img.getElementsByClassName('widget_image_border_left')[0];
	var border_right = img.getElementsByClassName('widget_image_border_right')[0];
	var border_bottom = img.getElementsByClassName('widget_image_border_bottom')[0];
	var corner_nw = img.getElementsByClassName('widget_image_corner_nw')[0];
	var corner_ne = img.getElementsByClassName('widget_image_corner_ne')[0];
	var corner_sw = img.getElementsByClassName('widget_image_corner_sw')[0];
	var corner_se = img.getElementsByClassName('widget_image_corner_se')[0];

	var moving = false;
	var position = []; // init mouse position when moving / resizing
	var resize_class = '';

	content.style.backgroundImage = "url('" + src + "') ";
	content.style.backgroundPosition = 'top left';
	content.style.backgroundRepeat = 'no-repeat';
	content.style.backgroundSize = 'cover';

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
		var min_w_size = 60; // min width size
		var min_h_size = 30; // min height size

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

	}
}

function w_image_switch_view(id, view)
{
	var img = document.getElementById(id);
	var content = img.getElementsByClassName('widget_image_content')[0];
	switch (view)
	{
		case 'cover':
			content.style.backgroundSize = 'cover';
			break;
		case 'contain':
			content.style.backgroundSize = 'contain';
			break;
	}
}

function init_widget_image(id, src)
{
	w_image(id, src);
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
