#!/bin/bash

TARGET="/server/web/peta/conf/nginx-peta.conf"
LINK="/etc/nginx/sites-enabled"

if [ ! -f "$TARGET" ]; then
	echo "nginx-peta.conf가 존재하지 않습니다"
	echo "${TARGET}을 확인해주세요"
	echo "스크립트를 종료합니다"
	exit 1
else
	if [ -L "$LINK/nginx-peta.conf" ]; then	
		echo "${LINK}에 nginx-peta.conf가 존재합니다" 
		sudo mv "$LINK/nginx-peta.conf" "$LINK/nginx-peta.disable"
		echo "Nginx peta conf 링크를 비활성화 했습니다"
	else
                echo "${LINK}에 링크가 존재하지 않습니다"
                sudo ln -s "$TARGET" "$LINK/nginx-peta.conf"
                echo "Nginx peta conf의 링크를 생성했습니다"
        fi

fi

sudo systemctl reload nginx
echo "Nginx 설정을 적용했습니다"
