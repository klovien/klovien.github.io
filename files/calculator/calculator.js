// anime({
//     targets: '#box1',
//     translateX: 250,
//     duration: 1250,
//     loop: true,
//     direction: 'alternate',
//     easing: 'linear'
// });



let buttons = document.querySelectorAll('.digits button[class="default"]');
let screen = document.querySelector('.screen');
let viceScreen = document.querySelector('.viceScreen');
let operators = document.querySelectorAll('.operators .operator');



buttons.forEach((button) => {
    let text = button.innerText;
    button.addEventListener('click', () => {
        let screenText = screen.value;

        if (screenText.includes('.') && text == '.') {
            return;
        }

        if (screenText == '0' && text != '.') {
            screenText = text;
        } else if (screenText.length >= 19) {
            screenText = "out of the range!"
        } else if (screenText == "out of the range!") {
            return;
        } else
            screenText += text;
        screen.value = screenText;
    })
})

function clearOut() {
    console.log('触发了"清空按钮"')
    if (screen.value == '0') {
        viceScreen.innerText = '';
    } else
        screen.value = '0';
}

function del() {
    console.log('触发了"删除按钮"');
    let res = screen.value;
    if (res == '0')
        return;
    res = res.substring(0, res.length - 1);
    if (res.length == 0) {
        screen.value = '0';
    } else
        screen.value = res;
}

function equal() {
    //副屏没有信息则返回
    if (viceScreen.innerHTML == '') {
        return;
    }
    screen.value = eval(viceScreen.innerHTML + screen.value);
    viceScreen.innerHTML = '';
}
for (let ele of operators) {

    let text = ele.innerText;
    ele.addEventListener('click', () => {
        console.log('触发了"操作符' + text + '"')
        let screenText = screen.value;

        //如果副屏已经有数，则先处理数
        if (viceScreen.innerHTML != '') {
            equal();
            return;
        }
        if (screenText == "out of the range!") {
            return;
        } else
            screenText += text;
        viceScreen.innerText = screenText;
        screen.value = '0';
    })
}