## 操作指南
- ### 1."Easy Diary"适配Windows与MacOS系统，在Windows系统下无须修改任何路径相关代码。在MacOS系统下，需要将 `FileOperation.cpp` 的第14行改为 `QString FileOperation::startPath=QDir::currentPath();` ，将 `.pro` 文件的 2-4 行删去，并注释掉 `markDownEditorWidget.cpp` 和 `markDownEditorWidget.h` 中报错的内容，再重新构建app。
- ### 2.项目中所用部分函数只能在QT6及以上版本运行。
- ### 3.markdown实时渲染编辑器由QT的QWebEngine包实现，因此需提前下载，并使用MSVC 2022编译器构建。若遇到报错，请删除.user文件，保留build文件夹，再点击pro文件重新构建。
- ### 4.build文件需使用github上下载的原版文件，因其中包含markdown预览所需的html与js文件，以及QWebEngine运行所需的dll文件。