void matrixmul() {
    int x[3][3] = {{1, 2, 3},{4,5,6},{7,8,9}};
    int y[3] = {5,2,1};
    int z[3] = {0,0,0};

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            int count = x[i][j] * y[j];
            z[i] = z[i] + count;
        }
    }
}
