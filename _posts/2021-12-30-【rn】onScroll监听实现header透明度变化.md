---
layout:     post
title:      【RN】onScroll监听实现header透明度变化
subtitle:   
date:       2021-12-30
author:     YeanSe
header-img: 
catalog: true
tags:
    - React
    - React-Native

---

```react
interface PropType {
  // route: any;
  // navigation: StackNavigationProp<ParamListBase, string>;
}

interface RouteParam {
  publisherInfo: PublisherInfoType;
}

const Profile: FC<PropType & PageProps> = props => {
  const {route, navigation} = props;
  const params = route.params as RouteParam;
  console.log('我将要处理下面这个', params.publisherInfo);

  const offSetY = useRef(0);
  useLayoutEffect(() => {
    navigation.setOptions({
      header: () => {
        return <Header opacity={0} quit={navigation.goBack} />;
      },
    });
  }, []);

  return (
    <>
      <ScrollView
        showsVerticalScrollIndicator={false}
        scrollEventThrottle={100}
        onScroll={event => {
          //计算距顶部高度
          offSetY.current = event.nativeEvent.contentOffset.y | 0;
          const maxY = global.gScreen.headerBarHeight + 100;//滑动响应的距离
          const opacity = Math.min(Math.max(offSetY.current / maxY, 0), 1);
        
        	//优化性能
          if (offSetY.current / maxY > 5) {
            return;
          } else if (offSetY.current / maxY < -2) {
            return;
          }

          navigation.setOptions({
             header: () => {
               return <Header opacity={opacity} quit={navigation.goBack} />;
             },
          });
        }}>

      </ScrollView>
    </>
  );
};
export default Profile;
```

