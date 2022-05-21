---
layout:     post
title:      Flume：Flume 零点漂移问题及解决方法示例
subtitle:   
date:       2022-01-28
author:     dex0423
header-img: img/post-bg-os-metro.jpg
catalog: true
tags:
    - Flume
---

##### 数据漂移问题

- 所谓零点漂移，就是在按天生成日志文件的情况下，一条23:59:59左右生成的日志发送到服务器后可能已经是第二天了，如果没有指定时间，会被写入第二天对应的文件中，这就是所谓的零点漂移。

- flume 写入日志到 HDFS 时，如果按照时间生成文件，在没有明确指定时间的情况下，会读取服务器时间作为创建文件的依据，这会导致日志的实际生成日期与文件不符。

##### 解决办法

- 这种情况下，可以通过拦截器在flume事件头指定timestamp作为文件的创建依据。

- 具体的通常是将日志中记录的日志创建时间提取出来，写入flume事件头的timestamp字段，有了这个字段，flume创建文件时，会依据这个字段创建文件，这种场景很类似spark、flink的事件事件和处理事件。



```aidl
@Override
    public Event intercept(Event event) {

        Map<String, String> headers = event.getHeaders();
        String log = new String(event.getBody(), StandardCharsets.UTF_8);

        JSONObject jsonObject = JSONObject.parseObject(log);

        String ts = jsonObject.getString("ts");
        headers.put("timestamp", ts);

        return event;
    }
```

```aidl
import com.alibaba.fastjson.JSONObject;
import org.apache.flume.Context;
import org.apache.flume.Event;
import org.apache.flume.interceptor.Interceptor;

import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.Map;

public class TimeStampInterceptor implements Interceptor {
    @Override
    public void initialize() {

    }
    @Override
    public Event intercept(Event event) {

        Map<String, String> headers = event.getHeaders();
        String log = new String(event.getBody(), StandardCharsets.UTF_8);

        JSONObject jsonObject = JSONObject.parseObject(log);

        String ts = jsonObject.getString("ts");
        headers.put("timestamp", ts);

        return event;
    }
    @Override
    public List<Event> intercept(List<Event> events) {

        for (Event event : events) {
            intercept(event);
        }

        return events;
    }
    @Override
    public void close() {

    }
    public static class Builder implements Interceptor.Builder {
        @Override
        public Interceptor build() {
            return new TimeStampInterceptor();
        }
        @Override
        public void configure(Context context) {
        }
    }
}
```