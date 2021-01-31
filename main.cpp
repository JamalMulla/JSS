#include "Video.h"
#include "src/base/array.h"
#include "src/base/processing_element.h"

int main() {

    ProcessingElement pe(6, 13);
    Array scamp(256, 256, pe);


    Video::capture();
    //Test::test_matrices();
    return 0;
}