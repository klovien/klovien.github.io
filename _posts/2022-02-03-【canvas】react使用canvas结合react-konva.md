---
layout:     post
title:      【canvas】用canvas结合react-konva
subtitle:   
date:       2022-02-03
author:     YeanSe
header-img: 
catalog: true
tags:
    - React
    - Canvas

---
```react
import React, { Component, useState, useRef, useCallback } from "react";
import Konva from "konva";
import { render } from "react-dom";
import { Stage, Layer, Rect, Text } from "react-konva";

const ThemeContext = React.createContext("red");

const MyRect = () => {
  const rectRef = useRef(null);
  console.log(rectRef);
  const handleDragStart = useCallback(() => {}, []);
  const handleDragEnd = useCallback(() => {
    const rect = rectRef.current;
    const x = rect.x();
    const y = rect.y();
    if (Math.abs(x - 50) < 200 && Math.abs(y - 200) < 200) {
      rect.to({
        x: 50,
        y: 200
      });
    }
  }, []);
  return (
    <Rect
      ref={rectRef}
      x={200}
      y={400}
      fill="blue"
      width={100}
      height={100}
      draggable
      onDragStart={handleDragStart}
      onDragEnd={handleDragEnd}
    ></Rect>
  );
};

const Canvas = () => {
  return (
    <ThemeContext.Consumer>
      {(value) => (
        <Stage width={window.innerWidth} height={window.innerHeight}>
          <ThemeContext.Provider value={value}>
            <Layer>
              <Rect x={50} y={200} fill="black" width={100} height={100}></Rect>

              <MyRect />
            </Layer>
          </ThemeContext.Provider>
        </Stage>
      )}
    </ThemeContext.Consumer>
  );
};

class App extends Component {
  render() {
    return (
      <ThemeContext.Provider value={"blue"}>
        <Canvas />
      </ThemeContext.Provider>
    );
  }
}

render(<App />, document.getElementById("root"));
```