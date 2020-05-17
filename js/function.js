'use strict';


/****    images    ****/

function w_image(id)
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
	var position = [];
	var resize_class = '';

	bar.onmousedown = on_bar_mousedown;

	function on_bar_mousedown(e)
	{
		e = e || window.event;
		e.preventDefault();
		if (e.buttons == 1) // left click
		{
			// get the mouse cursor position at startup
			position = [e.clientX, e.clientY];
			img.style.opacity = 0.7;
			document.onmouseup = on_bar_mouseup;
			// call a function whenever the cursor moves
			document.onmousemove = on_bar_mousemove;
		}
	}

	function on_bar_mouseup(e)
	{
		// stop moving when mouse button is released
		img.style.opacity = 1;
		document.onmouseup = null;
		document.onmousemove = null;
	}

	function on_bar_mousemove(e)
	{
		e = e || window.event;
		e.preventDefault();
		// calculate the new cursor position:
		var delta_x = e.clientX - position[0];
		var delta_y = e.clientY - position[1];

		// boundaries checks
		var safe_margin = 8;

		var new_position = [e.clientX, e.clientY];
		var new_top = img.offsetTop + delta_y;
		var new_left = img.offsetLeft + delta_x;

		if (img.offsetTop + delta_y < 0)
		{
			new_top = 0;
			position[1] = 0;
		}
		if (img.offsetTop + delta_y > window.innerHeight - safe_margin)
		{
			new_top = (window.innerHeight - safe_margin) + "px";
			new_position[1] = window.innerHeight - safe_margin;
		}
		if (img.offsetLeft + img.clientWidth + delta_x < safe_margin)
		{
			new_left = safe_margin + "px";
			new_position[0] = safe_margin;
		}
		if (img.offsetLeft + delta_x > window.innerWidth - safe_margin)
		{
			new_left = (window.innerWidth - safe_margin) + "px";
			new_position[0] = window.innerWidth - safe_margin;
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
		}
	}

	function on_border_mouseup(e)
	{
		// stop moving when mouse button is released
		console.log(resize_class);
		document.onmouseup = null;
		document.onmousemove = null;
	}

	function on_border_mousemove(e)
	{
		e = e || window.event;
		e.preventDefault();
		// calculate the new cursor position:
		var delta_x = e.clientX - position[0];
		var delta_y = e.clientY - position[1];

		// // boundaries checks
		// var safe_margin = 8;

		// var new_position = [e.clientX, e.clientY];
		// var new_top = img.offsetTop + delta_y;
		// var new_left = img.offsetLeft + delta_x;

		// if (img.offsetTop + delta_y < 0)
		// {
		// 	new_top = 0;
		// 	position[1] = 0;
		// }
		// if (img.offsetTop + delta_y > window.innerHeight - safe_margin)
		// {
		// 	new_top = (window.innerHeight - safe_margin) + "px";
		// 	new_position[1] = window.innerHeight - safe_margin;
		// }
		// if (img.offsetLeft + img.clientWidth + delta_x < safe_margin)
		// {
		// 	new_left = safe_margin + "px";
		// 	new_position[0] = safe_margin;
		// }
		// if (img.offsetLeft + delta_x > window.innerWidth - safe_margin)
		// {
		// 	new_left = (window.innerWidth - safe_margin) + "px";
		// 	new_position[0] = window.innerWidth - safe_margin;
		// }

		// position = new_position;

		// img.style.top = new_top + "px";
		// img.style.left = new_left + "px";
	}


}

function init_widget_image(id)
{
	w_image(id);
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
	var button_width = 150;
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
	context.fillText('CLEAR', button_width/2, canvas.height/2 + 3);

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
	context.fillText('THROW', radius_middle + button_width/2, canvas.height/2 + 3);
}
