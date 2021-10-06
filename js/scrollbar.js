"use strict";

/****    custom scrollbars for textarea & div    ****/

// options:
// bool autoscroll (default false)
function w_scrollarea(id, options = {})
{
	var area = document.getElementById(id);
	var bar = document.createElement('div');
	bar.setAttribute('class', 'sb-scroll');

	var p_area = area; // pointer to base element used to compute values
	var last_y;
	var total_h, area_h; // total content height, textarea height
	var travel; // travel distance for the bar, in pixel
	var bar_h; // bar height
	var bar_top; // bar distance from top
	var max_scroll; // maximim scrollTop
	var padding = 8; // padding top & bottom of bar
	var bar_right = 10; // scrollbar position right, in pixel

	// determine type, can be generic div, or textarea
	var type = 'div'; // default value
	if (area.nodeName.toLowerCase() == 'textarea') type = 'textarea';

	// options
	options.autoscroll = options.autoscroll || false;
	options.globalkey = options.globalkey || false;

	// insert a div wrapper, which will contain the area + scrollbar
	var wrapper = document.createElement('div');
	wrapper.setAttribute('class', 'sb-wrapper');

	if (type == 'textarea')
	{
		// single wrapper + scrollbar
		// swap area and wrapper, and add area to wrapper
		area.parentNode.insertBefore(wrapper, area);
		wrapper.appendChild(area);
		wrapper.appendChild(bar);
	}
	else
	{
		// wrapper + inner container + scrollbar
		// insert a div inner wrapper, which will contain the original area
		var inner = document.createElement('div');
		inner.setAttribute('class', 'sb-inner');

		// swap area and wrapper, and add area to wrapper
		area.parentNode.insertBefore(wrapper, area);
		wrapper.appendChild(inner);
		wrapper.appendChild(bar);
		inner.appendChild(area);

		p_area = inner;

		padding = 0; // padding top & bottom of bar
		bar_right = 0; // scrollbar position right, in pixel
	}

	// update wrapper width & height
	var cstyle = window.getComputedStyle(area);
	wrapper.style.cssText = 'width:100%;'
		+ 'height:' + parseInt(cstyle.getPropertyValue('height'), 10) + 'px;';

	// get line height
	var line_height = window.getComputedStyle(p_area).getPropertyValue('line-height');
	if (line_height.includes('px')) line_height = parseInt(line_height, 10);
	else if (line_height == 'normal')
		line_height = Math.round(1.2 * parseInt(window.getComputedStyle(p_area).getPropertyValue('font-size'), 10));

	on_area_change(); // init once

	bar.onmousedown = on_bar_mousedown;
	wrapper.onwheel = on_content_wheel;
	wrapper.onmouseenter = move_bar;
	area.onkeyup = on_area_change;
	if (options.globalkey) document.onkeydown = on_key_pressed;

	if (options.autoscroll)
	{
		var observer = new MutationObserver(scroll_down);
		observer.observe(area, { attributes: false, childList: true, subtree: true });
	}

	function scroll_down()
	{
		on_area_change(); // recompute values
		p_area.scrollTop = p_area.scrollHeight; // set max scroll
		move_bar(); // adjust bar position
	}

	function on_key_pressed(e)
	{
		switch (e.keyCode)
		{
			case 38: // up
				p_area.scrollTop += -3 * line_height;
				move_bar();
				break;
			case 40: // down
				p_area.scrollTop += 3 * line_height;
				move_bar();
				break;
			case 33: // pgup
				p_area.scrollTop += -40 * line_height;
				move_bar();
				break;
			case 34: // pgdown
				p_area.scrollTop += 40 * line_height;
				move_bar();
				break;
		}
	}

	function on_area_change()
	{
		total_h = p_area.scrollHeight;
		area_h = p_area.clientHeight;

		var ratio = area_h / total_h;
		if (ratio < 0.2) ratio = 0.2; // minimum bar ratio relative to area_h
		bar_h = ratio * area_h;
		travel = area_h - bar_h;

		travel -= 2 * padding;

		max_scroll = total_h - area_h;

		move_bar();
	}

	function on_bar_mousedown(e)
	{
		e = e || window.event;
		e.preventDefault();

		if (travel <= 0) return; // prevents divide by 0

		if (e.buttons == 1) // left click
		{
			// get the mouse cursor position at startup
			last_y = e.pageY;
			bar.classList.add("sb-grabbed");
			document.onmouseup = on_bar_mouseup;
			document.onmousemove = on_bar_mousemove;
		}
	}

	function on_bar_mousemove(e)
	{
		e = e || window.event;
		e.preventDefault();

		var delta = e.pageY - last_y;
		last_y = e.pageY;
		bar_top += delta;

		p_area.scrollTop = bar_top * max_scroll / travel;
		move_bar();
	}

	function on_bar_mouseup(e)
	{
		// stop moving when mouse button is released
		document.onmouseup = null;
		document.onmousemove = null;
		bar.classList.remove('sb-grabbed');
	}

	function on_content_wheel(e)
	{
		e = e || window.event;
		e.preventDefault();

		// wheel delta, negative is up, positive is down
		var delta_y = 0;
		if (e.deltaY) delta_y = e.deltaY;
		else if (e.wheelDelta) delta_y = -e.wheelDelta;
		// chrome fix
		if (delta_y < 0 && delta_y < -3) delta_y = -3;
		if (delta_y > 0 && delta_y > 3) delta_y = 3;

		// console.log(delta_y + " * "+ line_height);
		p_area.scrollTop += delta_y * line_height;
		move_bar();
	}

	function move_bar()
	{
		if (area_h >= total_h)
		{
			bar.classList.add('sb-hidden')
		}
		else
		{
			if (max_scroll <= 0) return; // prevents divide by 0
			bar.classList.remove('sb-hidden')
			bar_top = (p_area.scrollTop / max_scroll) * travel;

			if (bar_top > travel + padding) bar_top = travel + padding;
			else if (bar_top < 0) bar_top = 0;

			bar.style.cssText = 'height: ' + bar_h +'px; top:' + (bar_top + padding) + 'px;'
			+ 'right:' + bar_right + 'px;';
		}
	}
}

