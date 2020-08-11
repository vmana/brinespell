'use strict';

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

