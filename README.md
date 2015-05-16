#Builder
##Осуществляет построение 3d модели созданной [Transformer](https://github.com/SPC-project/Transformer)'ом

Проект собирается с помощью системы автоматизированной сборки [CMake](http://www.cmake.org/). Зависимости: [Qt4](http://www.qt.io/developers/). Для получения бинарного файла вам необходимо: 

1. [Подготовить проект](http://www.cmake.org/Wiki/CMake_Generator_Specific_Information), сгенерировав файл управления сборкой (make, Visual Studio, Code::Blocks, прочее). <i>Совет:</i> не забудьте указать тип сборки (debug/release). Пример (Linux, терминал): `cmake . -DCMAKE_BUILD_TYPE=Release`
2. Собрать проект, используя посредством используемой системы сборки