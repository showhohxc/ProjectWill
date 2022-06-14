# AddActorLocalOffset = 물체의 XYZ 방향에 따라 위치 이동
# AddActorWorldOffset = 지도의 XYZ 방향에 따라 위치 이동 (물체가 회전해도 고정)

# 이 경우 둘다 우측이 X 축이기 때문에 어떤것을 사용해도 
![img1 daumcdn](https://user-images.githubusercontent.com/98040028/168024741-3be1f3ba-cbdf-49e4-9eca-4bb2293f9c42.png)
# x축으로 이동하지만
![img1 daumcdn](https://user-images.githubusercontent.com/98040028/168024853-2adc9fb2-8a45-409f-bebb-f3003e510256.png)
# 이러한 방식으로 오브젝트를 돌려놓았을 경우 AddActorWorldOffset는 우측으로 200 AddActorLocalOffset 위로 200만큼 이동하게 된다.


# TSubclassOf<class CLASS> // 일반 Class 보다 안정선이 확보된 TSubclassof가 안전성이 높다.
