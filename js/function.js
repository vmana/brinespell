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

	// button left
	var canvas = document.getElementById('selector_clear_canvas');
	var context = canvas.getContext('2d');
	canvas.classList.add('selector_buttons');
	canvas.width = button_width + radius_middle;
	canvas.height = 140;
	canvas.style.left = 'calc(50% - ' + button_width + 'px - ' + radius_middle + 'px)';
	// canvas.style.bottom = 'calc(' + radius_middle + 'px - (' + canvas.height + 'px / 2))';
	canvas.style.bottom = '0px';

	context.fillStyle = '#0a141c';
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
	context.fillStyle = '#f9e9b8';
	context.fillText('CLEAR', button_width/2, canvas.height/2 + 3);

	// button right
	canvas = document.getElementById('selector_throw_canvas');
	context = canvas.getContext('2d');
	canvas.classList.add('selector_buttons');
	canvas.width = button_width + radius_middle;
	canvas.height = 140;
	canvas.style.right = 'calc(50% - ' + button_width + 'px - ' + radius_middle + 'px)';
	canvas.style.bottom = '0px';

	context.fillStyle = '#0a141c';
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
	context.fillStyle = '#f9e9b8';
	context.fillText('THROW', radius_middle + button_width/2, canvas.height/2 + 3);
}
