const pagetop = document.createElement('div');
pagetop.classList.add('pagetop');
document.body.insertBefore(pagetop, document.body.firstChild);

// innerHTML 대신 DOM 메서드 사용
const pageName = document.createElement('a');
pageName.href = '/';
pageName.id = 'pagename';
pageName.textContent = 'PETA';
pagetop.appendChild(pageName);

const service = document.createElement('div');
service.classList.add('service');
pagetop.appendChild(service);

const boardLink = document.createElement('a');
boardLink.href = '/board/board.html';
boardLink.textContent = '게시판';
boardLink.classList.add('board-link');
service.appendChild(boardLink);

const Account = document.createElement('div');
Account.classList.add('Account');

if (typeof user_id !== 'undefined' && user_id) {
// 텍스트 요소 생성
const welcomeText = document.createElement('span');
welcomeText.textContent = `${user_id}님 환영합니다`;
Account.appendChild(welcomeText);
    
    // 공백 추가
Account.appendChild(document.createTextNode(' '));
    
    // 내 정보 스팬 생성
const myInfo = document.createElement('span');
myInfo.id = 'myInfo';
myInfo.textContent = '내 정보';
myInfo.style.cursor = 'pointer';
Account.appendChild(myInfo);

    // 팝업 생성
const myInfoPopup = document.createElement('div');
myInfoPopup.id = 'myInfoPopup';
myInfoPopup.classList.add('popup');
myInfoPopup.style.display = 'none';
myInfoPopup.style.position = 'absolute';
  
const list = document.createElement('ul');
list.style.listStyle = 'none';
list.style.margin = '0';
list.style.padding = '5px';
    
const listItem = document.createElement('li');
const logoutLink = document.createElement('a');
logoutLink.href = '/Account/logout';
logoutLink.textContent = '로그아웃';
    
    listItem.appendChild(logoutLink);
    list.appendChild(listItem);
    myInfoPopup.appendChild(list);
    Account.appendChild(myInfoPopup);
    
    // 이벤트 리스너 (기존 코드 유지)
    myInfo.addEventListener("click", (e) => {
        e.stopPropagation();
        const rect = myInfo.getBoundingClientRect();
        myInfoPopup.style.display = "block";
        myInfoPopup.style.top = rect.bottom + window.scrollY + "px";
        myInfoPopup.style.left = rect.left + window.scrollX + "px";
    });

    myInfoPopup.addEventListener("mouseleave", () => {
        myInfoPopup.style.display = "none";
    });

    document.addEventListener("click", () => {
        myInfoPopup.style.display = "none";
    });

} else {
    // 로그인/회원가입 링크 생성
    const loginLink = document.createElement('a');
    loginLink.href = '/Account/login/login.html';
    loginLink.textContent = '로그인';
    Account.appendChild(loginLink);
    
    Account.appendChild(document.createTextNode(' '));
    
    const signupLink = document.createElement('a');
    signupLink.href = '/Account/signup/signup.html';
    signupLink.textContent = '회원가입';
    Account.appendChild(signupLink);
}

const Linetop = document.createElement('hr');
Linetop.id = 'Linetop';
document.body.insertBefore(Linetop, document.body.children[1]);

pagetop.appendChild(Account);
