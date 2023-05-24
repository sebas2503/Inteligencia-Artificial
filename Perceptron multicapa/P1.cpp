#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;


// Función de activación sigmoide o ReLU
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivada de la función de activación sigmoide
double sigmoidDerivative(double x) {
    double sig = sigmoid(x);
    return sig * (1 - sig);
}

// Clase del perceptrón multicapa
class MultiLayerPerceptron {
private:
    int numInputs;
    int numHidden;
    int numOutputs;

    std::vector<std::vector<double>> inputHiddenWeights;  // Pesos entre la capa de entrada y oculta
    std::vector<std::vector<double>> hiddenOutputWeights; // Pesos entre la capa oculta y de salida

public:
    MultiLayerPerceptron(int inputs, int hidden, int outputs) {
        numInputs = inputs;
        numHidden = hidden;
        numOutputs = outputs;

        // Inicializar los pesos con valores aleatorios entre -1 y 1 , permite que tenga valores tanto positivos como negativos
        inputHiddenWeights.resize(numHidden, std::vector<double>(numInputs));
        hiddenOutputWeights.resize(numOutputs, std::vector<double>(numHidden));

        srand(time(nullptr));
        for (int i = 0; i < numHidden; i++) {
            for (int j = 0; j < numInputs; j++) {
                inputHiddenWeights[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
            }
        }

        for (int i = 0; i < numOutputs; i++) {
            for (int j = 0; j < numHidden; j++) {
                hiddenOutputWeights[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
            }
        }
    }

    // Función de propagación hacia adelante
    std::vector<double> feedForward(const std::vector<double>& input) {
        std::vector<double> hiddenOutputs(numHidden);
        std::vector<double> outputs(numOutputs);

        // Calcular los valores de la capa oculta
        for (int i = 0; i < numHidden; i++) {
            double sum = 0.0;
            for (int j = 0; j < numInputs; j++) {
                sum += input[j] * inputHiddenWeights[i][j];
            }
            hiddenOutputs[i] = sigmoid(sum);
        }

        // Calcular los valores de la capa de salida
        for (int i = 0; i < numOutputs; i++) {
            double sum = 0.0;
            for (int j = 0; j < numHidden; j++) {
                sum += hiddenOutputs[j] * hiddenOutputWeights[i][j];
            }
            outputs[i] = sigmoid(sum);
        }

        return outputs;
    }

    // Función de retropropagación
    void backpropagation(const std::vector<double>& input, const std::vector<double>& target, double learningRate) {//actualiza los pesos del perceptrón cuando hay error entre la salida deseada y la resultante
        std::vector<double> hiddenOutputs(numHidden);
        std::vector<double> outputs(numOutputs);

        // Calcular los valores de la capa oculta y de salida
        for (int i = 0; i < numHidden; i++) {
            double sum = 0.0;
            for (int j = 0; j < numInputs; j++) {
                sum += input[j] * inputHiddenWeights[i][j];
            }
            hiddenOutputs[i] = sigmoid(sum);
        }

        for (int i = 0; i < numOutputs; i++) {
            double sum = 0.0;
            for (int j = 0; j < numHidden; j++) {
                sum += hiddenOutputs[j] * hiddenOutputWeights[i][j];
            }
            outputs[i] = sigmoid(sum);
        }

        // Calcular los errores de la capa de salida y de la capa oculta
        std::vector<double> outputErrors(numOutputs);
        std::vector<double> hiddenErrors(numHidden);

        for (int i = 0; i < numOutputs; i++) {
            outputErrors[i] = (target[i] - outputs[i]) * sigmoidDerivative(outputs[i]);
        }

        for (int i = 0; i < numHidden; i++) {
            double sum = 0.0;
            for (int j = 0; j < numOutputs; j++) {
                sum += outputErrors[j] * hiddenOutputWeights[j][i];
            }
            hiddenErrors[i] = sum * sigmoidDerivative(hiddenOutputs[i]);
        }

        // Actualizar los pesos entre la capa oculta y de salida
        for (int i = 0; i < numOutputs; i++) {
            for (int j = 0; j < numHidden; j++) {
                hiddenOutputWeights[i][j] += learningRate * outputErrors[i] * hiddenOutputs[j];
            }
        }

        // Actualizar los pesos entre la capa de entrada y oculta
        for (int i = 0; i < numHidden; i++) {
            for (int j = 0; j < numInputs; j++) {
                inputHiddenWeights[i][j] += learningRate * hiddenErrors[i] * input[j];
            }
        }
    }
};

int main() {
    // Crear y entrenar el perceptrón multicapa
    int numInputs = 30;
    int numHidden = 6;
    int numOutputs = 10;
    MultiLayerPerceptron mlp(numInputs, numHidden, numOutputs);

    // Datos de entrenamiento
    std::vector<std::vector<double>> trainingData =
    {
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, //Numero 0

       {0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0},//Numero 1

        {0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 1, 1, 0,
         0, 1, 0, 0, 0,
         0, 1, 1, 1, 0},//Numero 2

        {0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0},//Numero 3

        {0, 1, 0, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0},//Numero 4

        {0, 1, 1, 1, 0,
         0, 1, 0, 0, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0,
         0, 1, 1, 1, 0},//Numero 5

        {0, 1, 1, 1, 0,
         0, 1, 0, 0, 0,
         0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0},//Numero 6

        {1, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 0, 0}, //Numero 7

        {0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0},//Numero 8

        {0, 1, 1, 1, 0,
         0, 1, 0, 1, 0,
         0, 1, 1, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0,
         0, 0, 0, 1, 0}//Numero 9

    };
    std::vector<std::vector<double>> targets = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // Salida deseada para el número 0
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},   // Salida deseada para el número 1
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // Salida deseada para el número 2
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},   // Salida deseada para el número 3
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},   // Salida deseada para el número 4
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},   // Salida deseada para el número 5
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},   // Salida deseada para el número 6
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // Salida deseada para el número 7
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},   // Salida deseada para el número 8
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   // Salida deseada para el numero 9
    };

    // Parámetros de entrenamiento
    double learningRate = 1.0;
    int numEpochs = 1000;

    // Entrenamiento del perceptrón multicapa
    for (int epoch = 0; epoch < numEpochs; epoch++) {
        double totalError = 0.0;

        for (int i = 0; i < trainingData.size(); i++) {
            // Obtener la entrada y el objetivo correspondiente
            std::vector<double> input = trainingData[i];
            std::vector<double> target = targets[i];

            // Realizar la retropropagación
            mlp.backpropagation(input, target, learningRate);

            // Calcular el error
            std::vector<double> output = mlp.feedForward(input);
            double error = 0.0;
            for (int j = 0; j < numOutputs; j++) {
                error += 0.5 * pow(target[j] - output[j], 2);
            }
            totalError += error;
        }

        // Mostrar el error promedio por época
        //double averageError = totalError / trainingData.size();
        //std::cout << "Epoch: " << epoch + 1 << ", Error: " << averageError << std::endl;
    }

    // Ejemplo de predicción
    std::vector<double> input3;
    std::ifstream is("Test.txt");
    /*char val = ' ';
    while(!is.eof())
    {
        is.get(val);
        input3.push_back((double(val))-48);
    }*/
    string linea;
    while (getline(is, linea))
    {
        for (int i = 0; i < linea.size(); i++)
        {
            input3.push_back(double(linea[i]) - 48);
        }
        std::vector<double> output = mlp.feedForward(input3);
        std::cout << "Predicted: " << std::endl;
        for (int i = 0; i < numOutputs; i++)
        {
            std::cout << "Num: " << i << "->";
            std::cout << output[i] << " " << std::endl;
        }
        std::cout << std::endl;
        input3.clear();
        linea.clear();
    }
    is.close();
    //Matriz 6x5;
    /*std::vector<double> output = mlp.feedForward(input3);
    std::cout << "Predicted: "<<std::endl;
    for (int i = 0; i < numOutputs; i++)
    {
        std::cout<<"Num: "<<i<<"->";
        std::cout << output[i] << " "<<std::endl;
    }
    std::cout << std::endl;*/

    return 0;
}





