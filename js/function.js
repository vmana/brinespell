/****    chat    ****/

function init_chat_box(wt_chat_id)
{
	var chat = document.getElementById(wt_chat_id);
	// change scrollbar display
	SimpleScrollbar.initEl(chat);

	const callback = function()
	{
		var scroll_id = chat.getElementsByClassName('ss-content');
		// console.log(scroll_id[0].scrollTop + ":" + scroll_id[0].scrollHeight);
		scroll_id[0].scrollTop = scroll_id[0].scrollHeight;
	};
	var observer = new MutationObserver(callback);
	observer.observe(chat, { attributes: false, childList: true, subtree: true });
}

/****    custom buttons    ****/

function xxx()
{
	var context = document.getElementById('canvas_button').getContext('2d');
	var canvas = document.getElementById('canvas_button');
	context.fillStyle = '#0a141c';
	context.beginPath();
	// right circle
	context.arc(canvas.width - (canvas.height / 2), (canvas.height / 2), (canvas.height / 2), 0, 2 * Math.PI, false);
	// rectangle button
	context.fillRect(0, 0, canvas.width - (canvas.height / 2), canvas.height);
	context.fill();
	// remove half a circle of the left
	context.save();
	context.globalCompositeOperation = 'destination-out';
	context.beginPath();

	var left_radius = 100;
	var radius_x = -80;

	context.arc(radius_x, (canvas.height / 2), left_radius, 0, 2 * Math.PI, false);
	context.fill();
	context.restore();
}
