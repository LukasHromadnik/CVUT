struct Example {
    int numberOfNodes;
    double ** input;
};

Example initializeAndSetupFirstExample() {
    int numberOfNodes = 4;

    double ** input = initializeMatrix(numberOfNodes, 0.0);
    input[0][0] = 5;
    input[0][1] = 9;
    input[0][2] = 3;
    input[0][3] = 6;
    input[1][0] = 8;
    input[1][1] = 7;
    input[1][2] = 8;
    input[1][3] = 2;
    input[2][0] = 6;
    input[2][1] = 10;
    input[2][2] = 12;
    input[2][3] = 7;
    input[3][0] = 3;
    input[3][1] = 10;
    input[3][2] = 8;
    input[3][3] = 6;

    Example result = Example();
    result.numberOfNodes = numberOfNodes;
    result.input = input;

    return result;
}

Example initializeAndSetupSecondExample() {
    int numberOfNodes = 5;

    double ** input = initializeMatrix(numberOfNodes, 0.0);
    input[0][0] = 11;
    input[0][1] = 7;
    input[0][2] = 10;
    input[0][3] = 17;
    input[0][4] = 10;
    input[1][0] = 13;
    input[1][1] = 21;
    input[1][2] = 7;
    input[1][3] = 11;
    input[1][4] = 13;
    input[2][0] = 13;
    input[2][1] = 13;
    input[2][2] = 15;
    input[2][3] = 13;
    input[2][4] = 14;
    input[3][0] = 18;
    input[3][1] = 10;
    input[3][2] = 13;
    input[3][3] = 16;
    input[3][4] = 14;
    input[4][0] = 12;
    input[4][1] = 8;
    input[4][2] = 16;
    input[4][3] = 19;
    input[4][4] = 10;

    Example result = Example();
    result.numberOfNodes = numberOfNodes;
    result.input = input;

    return result;
}

Example initializeAndSetupThirdExample() {
    int numberOfNodes = 4;

    double ** input = initializeMatrix(numberOfNodes, 0.0);
    input[0][0] = 3;
    input[0][1] = 1;
    input[0][2] = 1;
    input[0][3] = 4;

    input[1][0] = 4;
    input[1][1] = 2;
    input[1][2] = 2;
    input[1][3] = 5;

    input[2][0] = 5;
    input[2][1] = 3;
    input[2][2] = 4;
    input[2][3] = 8;

    input[3][0] = 4;
    input[3][1] = 2;
    input[3][2] = 5;
    input[3][3] = 9;

    Example result = Example();
    result.numberOfNodes = numberOfNodes;
    result.input = input;

    return result;
}

Example initializeAndSetupFourthExample() {
    int numberOfNodes = 3;

    double ** input = initializeMatrix(numberOfNodes, 0.0);
    input[0][0] = 4;
    input[0][1] = 16;
    input[0][2] = 9;

    input[1][0] = 81;
    input[1][1] = 121;
    input[1][2] = 25;

    input[2][0] = 36;
    input[2][1] = 100;
    input[2][2] = 1;

    Example result = Example();
    result.numberOfNodes = numberOfNodes;
    result.input = input;

    return result;
}

Example initializeAndSetupFifthExample() {
    int numberOfNodes = 3;

    double ** input = initializeMatrix(numberOfNodes, 0.0);
    input[0][0] = 2;
    input[0][1] = 4;
    input[0][2] = 3;

    input[1][0] = 9;
    input[1][1] = 11;
    input[1][2] = 5;

    input[2][0] = 6;
    input[2][1] = 10;
    input[2][2] = 1;

    Example result = Example();
    result.numberOfNodes = numberOfNodes;
    result.input = input;

    return result;
}