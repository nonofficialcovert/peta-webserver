// 게시글 목록을 가져와서 렌더링하는 함수
async function loadPosts() {
	const board = document.getElementById('notice-write');

	try {
	// API 호출
	const response = await fetch('http://192.168.0.9/api/notice-board');
	if (!response.ok) throw new Error('게시글을 불러오지 못했습니다.');

	const data = await response.json();
	const posts = data.posts;

	if (posts.length === 0) {
	board.innerHTML = '<p style="text-align:center; color:#666;">등록된 글이 없습니다.</p>';
	return;
	}

    // 테이블 생성
    const table = document.createElement('table');
    table.style.width = '100%';
    table.style.borderCollapse = 'collapse';
    table.style.marginTop = '20px';

    // 테이블 헤더
    const thead = document.createElement('thead');
    thead.innerHTML = `
      <tr style="border-bottom: 2px solid #333;">
        <th style="padding: 12px; text-align: left;">번호</th>
        <th style="padding: 12px; text-align: left;">제목</th>
        <th style="padding: 12px; text-align: center;">작성일</th>
      </tr>
    `;
    table.appendChild(thead);

    // 테이블 바디
    const tbody = document.createElement('tbody');
    posts.forEach(post => {
      const tr = document.createElement('tr');
      tr.style.borderBottom = '1px solid #ddd';
      tr.style.cursor = 'pointer';
      tr.onclick = () => location.href = `/board/view.html?id=${post.id}`; // 상세 페이지 이동

      tr.innerHTML = `
        <td style="padding: 12px; text-align: left;">${post.id}</td>
        <td style="padding: 12px; text-align: left;">
          <a href="/board/view.html?id=${post.id}" style="text-decoration:none; color:#333;">
            ${post.title}
          </a>
        </td>
        <td style="padding: 12px; text-align: center;">${formatDate(post.date)}</td>
      `;
      tbody.appendChild(tr);
    });
    table.appendChild(tbody);

    board.appendChild(table);

  } catch (error) {
    console.error('Error:', error);
    board.innerHTML = '<p style="color:red; text-align:center;">게시글을 불러오는 중 오류가 발생했습니다.</p>';
  }
}


// 날짜 포맷팅 (YYYY-MM-DD)
function formatDate(dateString) {
  const date = new Date(dateString);
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, '0');
  const day = String(date.getDate()).padStart(2, '0');
  return `${year}-${month}-${day}`;
}

// 페이지 로드 시 실행
document.addEventListener('DOMContentLoaded', loadPosts);
