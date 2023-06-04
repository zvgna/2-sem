#include <iostream>
#include <string>
#include <chrono>
#include <fstream> 
#include <Windows.h>

using namespace std;
using namespace chrono;
struct Node {
	int data;
	Node* left, * right;
};

struct Trunk {
	Trunk* prev;
	string str;
	Trunk(Trunk* prev, string str) {
		this->prev = prev;
		this->str = str;
	}
};

// Палочки-веточки
void showTrunks(Trunk* (&trunk)) {
	if (trunk == NULL) return;
	showTrunks(trunk->prev);
	cout << trunk->str;
}

void insert(Node* (&root), int data) {
	if (root == NULL) {
		root = new Node;
		root->data = data;
		root->left = root->right = NULL;
	}
	else {
		if (root->data == data) return;
		if (data < root->data) insert(root->left, data);
		else insert(root->right, data);
	}
}

// Создание дерева с случайными
void createRandTree(Node* (&tree), int amount) {
	int data;
	auto start = steady_clock::now();
	for (int i = 0; i < amount; i++) {
		data = 99 - (rand() % 199);
		insert(tree, data);
	}
	auto end = steady_clock::now();
	cout << "Время создания " << duration_cast<nanoseconds>(end - start).count() << " наносекунд\n";
}

// Создание дерева с консоли
void createInpTree(Node* (&tree)) {
	int inp;
	auto start = steady_clock::now();
	do {
		cout << "Введите элементs дерева: ";
		cin >> inp;
		insert(tree, inp);
	} while (cin.get() != '\n');
	auto end = steady_clock::now();
	cout << "Время создания " << duration_cast<nanoseconds>(end - start).count() << " наносекунд\n";
}

// Создание дерева из файла
void createFileTree(Node* (&tree)) {
	ifstream database("tree.txt");
	if (!database.is_open())
		cout << "Ошибочка открытия";
	else
	{
		int data;
		auto start = steady_clock::now();
		while (!database.eof())
		{
			database >> data;
			insert(tree, data);
		}
		database.close();
		auto end = steady_clock::now();
		cout << "Время создания " << duration_cast<nanoseconds>(end - start).count() << " наносекунд\n";
	}
}

// Вывод дерева
void printTree(Node* (&root), Trunk* prev, bool isRight) {
	if (root == NULL) return;
	string prev_str = "    ";
	Trunk* trunk = new Trunk(prev, prev_str);
	printTree(root->right, trunk, true);
	if (!prev) {
		trunk->str = "--->";
	}
	else if (isRight) {
		trunk->str = ".-->";
		prev_str = "   |";
	}
	else {
		trunk->str = "`-->";
		prev->str = prev_str;
	}
	showTrunks(trunk);
	cout << root->data << endl;
	if (prev) prev->str = prev_str;
	trunk->str = "   |";
	printTree(root->left, trunk, false);
}

// Максимальное значение
int maxData(Node* (&root)) {
	if (root->right == NULL) return root->data;
	else return maxData(root->right);
}

// Удалить элемент
void deleateElement(Node* (&root), int data) {
	if (root) {
		if (data > root->data)
			deleateElement(root->right, data);
		else if (data < root->data)
			deleateElement(root->left, data);
		else {
			if (root->left == 0 && root->right == 0) {
				delete root;
				root = 0;
			}
			else if (root->right == 0) {
				Node* del = root;
				root = root->left;
				delete del;
			}
			else if (root->left == 0) {
				Node* del = root;
				root = root->right;
				delete del;
			}
			else {
				int max = maxData(root->left);
				root->data = max;
				deleateElement(root->left, max);
			}
		}
	}
	else {
		cout << "Данного элемента нет в дереве\n";
		return;
	}
}

// Получить эллемент
void getElement(Node* (&root), int data) {
	if (root == NULL) {
		cout << "Элемент не найден";
		return;
	}
	if (data < root->data) getElement(root->left, data);
	else if (data > root->data) getElement(root->right, data);
	else cout << "Значение: " << root->data << "\tАдрес в памяти: " << root << endl;
}

// Удаление
void deleteTree(Node* (&root))
{
	if (root != NULL)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
		root = NULL;
	}
}

// Веточки
void trunksToFile(Trunk* (&trunk), ofstream& file) {
	if (trunk == NULL) return;
	trunksToFile(trunk->prev, file);
	file << trunk->str;
}

// Деревце в файлик

void treeToFile(Node* (&root), Trunk* prev, bool isRight, ofstream& file) {
	if (root == NULL) return;
	string prev_str = "    ";
	Trunk* trunk = new Trunk(prev, prev_str);
	treeToFile(root->right, trunk, true, file);
	if (!prev) {
		trunk->str = "--->";
	}
	else if (isRight) {
		trunk->str = ".-->";
		prev_str = "   |";
	}
	else {
		trunk->str = "`-->";
		prev->str = prev_str;
	}
	trunksToFile(trunk, file);
	file << root->data << endl;
	if (prev) prev->str = prev_str;
	trunk->str = "   |";
	treeToFile(root->left, trunk, false, file);
}

// Задачки
void tasks(int tasks) {
	ofstream fileTask;
	ofstream fileKey;
	ofstream fileAns;
	fileTask.open("output_task.txt");
	fileKey.open("output_key.txt");
	fileAns.open("output_ans.txt");
	if (!fileTask.is_open() || !fileKey.is_open() || !fileAns.is_open()) cout << "Ошибка открытия файла.\n";
	else {
		int amount, del, ins;
		Node* tree = NULL;
		SetConsoleCP(1251);
		for (int i = 0; i < tasks; i++) {
			amount = (1 + (rand() % 100));
			del = (99 - (rand() % 199));
			ins = (99 - (rand() % 199));
			fileTask << "Задание:\n" << "Создать дерево размером " << amount << ". \nУдалить элемент со значением " << del << ". \nВставить элемент со значением " << ins << ".\n\n";
			fileKey << "\tЗадание " << (i + 1) << endl;
			fileAns << "\tЗадание " << (i + 1) << endl;
			fileAns << "Создано дерево размером " << amount << ".\n";
			createRandTree(tree, amount);
			treeToFile(tree, NULL, false, fileAns);
			fileAns << "\nУдаление числа " << del << ".\n";
			deleateElement(tree, del);
			treeToFile(tree, NULL, false, fileAns);
			fileAns << "\nДобавление числа " << ins << ".\n";
			insert(tree, ins);
			treeToFile(tree, NULL, false, fileAns);
			treeToFile(tree, NULL, false, fileKey);
			fileAns << "\n\n";
			fileKey << "\n\n";
			deleteTree(tree);
		}
		SetConsoleCP(866);
		cout << "Задания выполнены.\n";
	}
}

// Еще Задачки
void Tasks() {
	int num;
	cout << "В каждом задании будет создано дерево размером от 1 до 100 элементов.\n"
		<< "Будет выбрано случайное число, которое будет удалено.\n"
		<< "Будет выбрано случайное число, которое добавится в дерево.\n"
		<< "Введите, сколько заданий сгенерировать: ";
	cin >> num;
	tasks(num);
}

// Промой обход
void preOrder(Node* root) {
	if (root) {
		cout << root->data << " ";
		preOrder(root->left);
		preOrder(root->right);
	}
}

// Симметричный
void inOrder(Node* root) {
	if (root) {
		inOrder(root->left);
		cout << root->data << " ";
		inOrder(root->right);
	}
}

// Обратный
void postOrder(Node* root) {
	if (root) {
		postOrder(root->left);
		postOrder(root->right);
		cout << root->data << " ";
	}
}

int main() {
	setlocale(LC_ALL, "rus");
	Node* tree = NULL;
	char menu, create, trav;
	int amount, data;
	while (true) {
		cout << "Меню:\n"
			<< "1. Создать новое дерево\n"
			<< "2. Вывод бинарного дерева\n"
			<< "3. Вставить элемент в дерево\n"
			<< "4. Удалить элемент из дерева\n"
			<< "5. Получить элемент дерева.\n"
			<< "6. Сгенерировать практические задания.\n"
			<< "7. Совершить обход дерева.\n"
			<< "0. Выйти из программы\n"
			<< "Ваш выбор: ";
		cin >> menu;
		switch (menu) {
		case '1':
			system("cls");
			if (tree) deleteTree(tree);
			cout << "1. Заполнить дерево случайного числа\n"
				<< "2. Ввести элементы дерева вручную\n"
				<< "3. Заполнить данными из массива.\n"
				<< "Ваш выбор: ";
			cin >> create;
			switch (create) {
			case '1':
				cout << "Введите количество элементов дерева: ";
				cin >> amount;
				createRandTree(tree, amount);
				cout << "Создано дерево:\n";
				printTree(tree, NULL, false);
				break;
			case '2':
				createInpTree(tree);
				cout << "Создано дерево:\n";
				printTree(tree, NULL, false);
				break;
			case '3':
				createFileTree(tree);
				cout << "Создано дерево:\n";
				printTree(tree, NULL, false);
				break;
			default:
				cout << "Некорректный выбор. Вы будете возвращены в меню.\n";
			}
			system("pause");
			system("cls");
			break;
		case '2':
			system("cls");
			if (tree) printTree(tree, NULL, false);
			else cout << "Дерево не создано.\n";
			system("pause");
			system("cls");
			break;
		case '3':
			system("cls");
			if (tree) {
				printTree(tree, NULL, false);
				cout << "Введите элемент, который хотите добавить: ";
				cin >> data;
				auto start = steady_clock::now();
				insert(tree, data);
				cout << "Элемент добавлен.\n";
				auto end = steady_clock::now();
				cout << "Время добавления " << duration_cast<nanoseconds>(end - start).count() << " наносекунд\n";
				printTree(tree, NULL, false);
			}
			else cout << "Дерево не создано.\n";
			system("pause");
			system("cls");
			break;
		case '4':
			system("cls");
			if (tree) {
				printTree(tree, NULL, false);
				cout << "Введите элемент, который хотите удалить: ";
				cin >> data;
				auto start = steady_clock::now();
				deleateElement(tree, data);
				cout << "Элемент удалён.\n";
				auto end = steady_clock::now();
				cout << "Время удаления " << duration_cast<nanoseconds>(end - start).count() << " наносекунд\n";
				printTree(tree, NULL, false);
			}
			else cout << "Дерево не создано.\n";
			system("pause");
			system("cls");
			break;
		case '5':
			system("cls");
			if (tree) {
				printTree(tree, NULL, false);
				cout << "Введите элемент, который хотите получить: ";
				cin >> data;
				auto start = steady_clock::now();
				getElement(tree, data);
				auto end = steady_clock::now();
				cout << "Время получения " << duration_cast<nanoseconds>(end - start).count() << " наносекунд\n";
			}
			else cout << "Дерево не создано.\n";
			system("pause");
			system("cls");
			break;
		case '6':
			system("cls");
			Tasks();
			system("pause");
			system("cls");
			break;
		case '7':
			system("cls");
			cout << "1. Прямой обход\n"
				<< "2. Симметричный обход\n"
				<< "3. Обратный обход\n"
				<< "Ваш выбор: ";
			cin >> trav;
			switch (trav) {
			case '1':
				preOrder(tree);
				break;
			case '2':
				inOrder(tree);
				break;
			case '3':
				postOrder(tree);
				break;
			default:
				cout << "Некорректный выбор. Вы будете возвращены в меню.\n";
			}
			system("pause");
			system("cls");
			break;
		case '0':
			return 0;
		default:
			cout << "Пункт выбран неправильно!\n";
			system("pause");
			system("cls");
			break;
		}
	}
	return 0;
}