# GJFParser
## 简介

输入guassview的.gjf文件，输出.com文件和.pbs文件


## 命令

```bash
gen [option] [filename] [-charge] 电荷数 [-spin] 自旋多重度
```

[option] 参数
>[option]参数与[filename]参数为必填项，[-charge]和[-spin]为选填项，如果你不手动输入[-charge]或者[-spin]，会默认读取你的gjf文件里的电荷和自选多重度作为默认值

- -opt：生成**优化+频率计算**的.com文件
- -ts：生成**寻找过渡态**的.com文件
- -charge：体系所带电荷
- -spin：体系自旋多重度

[filename]参数

- .gjf文件名

## com.config配置文件的配置项

- OPT_METHOD：不带过渡金属opt时的默认方法+基组
- METAL_OPT_METHOD：不带过渡金属opt时的默认方法+基组
- TS_METHOD：带过渡金属找ts时的默认方法+基组
- METAL_TS_METHOD：带过渡金属找ts时的默认方法+基组



## 注意点

1. 你无需考虑体系中是否含有过渡金属元素，程序内置一张主族元素与过渡金属元素的表，会自动分类你的gjf文件中的元素并生成对应的.com文件
2. 你可以通过修改com.config配置文件自定义基组和方法





