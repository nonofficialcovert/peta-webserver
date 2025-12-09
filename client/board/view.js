const params = new URLSearchParams(window.location.search);
const postID = params.get('id');
async function loadPost()
{
	try {
		const response = await fetch(`http://192.168.0.9/api/notice-view?id=${postID}`);
		if (!response.ok) throw new Error('게시글을 불러오지 못했습니다.');
		const post = await response.json();
		document.getElementById('title').innerHTML = post.title || '(제목 없음)';
		document.getElementById('contents').innerHTML = post.content || '(내용 없음)';
	} catch (error) {
		alert(error.message);
	}
}
loadPost();

