import requests
import json

# 도시 이름, API 키, 언어 설정
city = "Seoul"
apikey = "ef6f7907a98fe53c6f41120e4f6cfbb7"
lang = "kr"

# API 호출 URL
api = f"https://api.openweathermap.org/data/2.5/forecast?q={city}&appid={apikey}&lang={lang}&units=metric"

try:
    # API 요청
    result = requests.get(api)
    # 상태 코드 확인
    if result.status_code == 200:
        # JSON 응답 파싱
        data = result.json()
        
        # 결과를 텍스트 파일로 저장
        with open('weather_forecast.txt', 'w', encoding='utf-8') as file:
            file.write(json.dumps(data, indent=4, ensure_ascii=False))
        
        print("날씨 예측 정보가 'weather_forecast.txt' 파일에 저장되었습니다.")
    else:
        print(f"API 요청 실패: {result.status_code} {result.reason}")
        
except requests.exceptions.RequestException as e:
    print(f"요청 중 오류 발생: {e}")