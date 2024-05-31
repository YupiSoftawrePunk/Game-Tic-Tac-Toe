#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

bool svob[9] = { false };		// эта переменная нужна для показа пустых ячеек на поле

class Stav		// Этот класс делает элементы, которыми мы будем управлять
{				// Крестики и нолики - это одинаковые объекты
public:
	Sprite sprite[9];		// Создадим новую переменныю, которая будет использовать масиив 9, так как поле состоит из 9 клеток
	bool tik[9];			// Позволяет сделать элементы видимыми или нет

	Stav(Texture& image)
	{
		for (int i = 0; i < 9; i++)
		{
			sprite[i].setTexture(image);	// Сдесь мы загружаем текстуру из ссылки
			tik[i] = false;			/* tik = false, потому что в начале игры эти элементы не будут видны */
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)			// Пишем для каждого спрайта свою позицию
			{
				sprite[i * 3 + j].setPosition(200 * j, 200 * i);
			}
		}
	}

	void update(int& vid)    // С помощью этой функции мы берем нужный кусок картинки
	{
		for (int i = 0; i < 9; i++)				// рисует либо х либо о
		{
			sprite[i].setTextureRect(IntRect(200 * (vid - 1), 0, 200, 200));
		}
	}
};

int BotStav()	// функция, которая проверяет свободные ячейки для хода бота
{
	int res = 0;

	bool verno = false;		// Эта переменная определяет нашел ли бот свободную ячейку
	while (!verno)		// ищет ячейку
	{
		res = rand() % 9;	// выбирает любую ячейку

		if (!svob[res])		// ячейка свободна?
		{
			verno = true;	// если да, то выход из цикла
		}
		else
		{
			verno = false;	// если нет, то продолжаем искать свободную ячейку
		}
	}

	return res;
}

int main() 
{
	srand(time(0));
	RenderWindow window(VideoMode(600, 600), "Tic tac toe!");     // Создаем окно

	Texture f;
	f.loadFromFile("fon.png");		// Подгружаем поле игры
	Sprite fon(f);

	Texture c;
	c.loadFromFile("xo1.png");			// Выбор игрока крестик или нолик
	Sprite vibor[2];
	for (int i = 0; i < 2; i++) 
	{
		vibor[i].setTexture(c);
		vibor[i].setPosition(50 + 300 * i, 180);
	}
	int vib = 0;
	Stav player(c), bot(c);

	int sv = 0;		// эта переменная считает сколько заполнено ячеек

	Texture l;
	l.loadFromFile("line.png");		// подгружаем текстуру линии, которая отвечает за выигрыш
	Sprite line(l);
	bool win = false, hotbot = false;		//переменная win отвечает за отрисовку линии, переменная hotbot отвечает может ли бот сделать ход
	bool dumbot = false;

	while (window.isOpen())
	{
		Vector2i pos = Mouse::getPosition(window);

		Event event;
		while (window.pollEvent(event))    // после условия закрытия окна 
		{

			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == Event::MouseButtonPressed)		// если мы управляем мышкой и выберем либо x либо o
			{
				if (event.key.code == Mouse::Left)				// то мы изменяем переменную vib на 1, либо на 2
				{
					if (vib == 0)
					{					// Тут происходит сам выбор игрока, если не выбрал, то выбирает
						for (int i = 0; i < 2; i++)
						{
							if (vibor[i].getGlobalBounds().contains(pos.x, pos.y))
							{
								vib = i + 1;
							}
						}
					}
					else
					{
						for (int i = 0; i < 9; i++)
						{
							if (player.sprite[i].getGlobalBounds().contains(pos.x, pos.y) && !svob[i])
							{
								player.tik[i] = true;		// Если игрок сделал выбор, то программа начинает отрисовывать объекты
								svob[i] = true;
								sv++;				// после хода свободная ячейка заполняется

								hotbot = true;
							}
						}
					}
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) // выход из игры тут
		{
			break;
		}

		for (int i = 0; i < 2; i++)		/* Здесь задается условие, если курсор находится либо на крестике, либо на нолике
											то тогда то или иное изображение меняется*/
		{
			if (vibor[i].getGlobalBounds().contains(pos.x, pos.y))
			{
				vibor[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
			}
			else
			{
				vibor[i].setTextureRect(IntRect(200 * i, 0, 200, 200));
			}
		}	
		
		bool winplay[8][2];			// эта переменная отвечает за событие, т.е. 3 по горизонтали, по вертикали или по диагонали
		for (int i = 0; i < 8; i++)		// также эта линия отнсится либо к игроку, либо к боту
		{							// предусмотрено 8 случаев выигрыша
			if (i < 3) 
			{								// этот случай для выигрыша по горизонтали
				winplay[i][0] = player.tik[3 * i] && player.tik[1 + 3 * i] && player.tik[2 + 3 * i]; // событие, если игрок выиграл
				winplay[i][1] = bot.tik[3 * i] && bot.tik[1 + 3 * i] && bot.tik[2 + 3 * i];		// событие, если бот выиграл
			}
			else if (i >= 3 && i < 6)		// этот случай для выигрыша по вертикали
			{
				winplay[i][0] = player.tik[i - 3] && player.tik[i] && player.tik[i + 3];	// выигрыш игрока
				winplay[i][1] = bot.tik[i - 3] && bot.tik[i] && bot.tik[i + 3];			//выигрыш бота
			}
			else if (i >= 6)	// этот случай для выигрыша по диагонали
			{
				winplay[i][0] = player.tik[2 * (i - 6)] && player.tik[4] && player.tik[8 - 2 * (i - 6)];	//выигрыш игрока
				winplay[i][1] = bot.tik[2 * (i - 6)] && bot.tik[4] && bot.tik[8 - 2 * (i - 6)];			// выигрыш бота
			}

			for (int j = 0; j < 2; j++)		// если сработало какое-то событие, то рисуем определенную линию
			{
				if (winplay[i][j])
				{
					win = true;
					if (i < 3)
					{							// линия рисуется по горизонтали
						line.setTextureRect(IntRect(0, 0, 600, 10));
						int ly = 95 + 200 * i;
						line.setPosition(0, ly);
					}
					else if (i < 6)
					{							// линия рисуется по вертикали
						line.setTextureRect(IntRect(0, 0, 600, 10));
						line.setRotation(90);
						int lx = 105 + 200 * (i - 3);
						line.setPosition(lx, 0);
					}
					else if (i == 6)			// линия рисуется по диагонали в одну и в другую сторону
					{
						line.setTextureRect(IntRect(0, 10, 600, 600));
					}
					else if (i == 7)
					{
						line.setTextureRect(IntRect(600, 10, -600, 600));
					}
				}
			}
		}

		if (hotbot && !win && sv < 9) // условие, из-за которого бот не может делать дальше ход
		{								// Может ли бот идти дальше, или пустые клетки кончились, или игрок уже выиграл
			sv++;

			int botstav = 0;

			if (!dumbot) 
			{
				botstav = BotStav();		// если переменная ложь, то тогда бот ищет свободную ячейку
				dumbot = true;
			}
			else 
			{
				bool cdat = true;

				for (int i = 0; i < 3; i++)			// логика по которой бот делает ход
				{
					if (player.tik[i] && player.tik[3 + i] && !svob[6 + i]) //если крестики стоят по вертикали, а внизу ячейка свободна, то бот ходит в ту ячейку
					{
						botstav = 6 + i;
						cdat = false;
					}
					if (player.tik[6 + i] && player.tik[3 + i] && !svob[i])  //если крестики стоят по вертикали, а вверху ячейка свободна, то бот ходит в ту ячейку
					{
						botstav = i;
						cdat = false;
					}
					if (player.tik[3 * i] && player.tik[1 + 3 * i] && !svob[2 + 3 * i]) // если 2 х стоят по горизонтали и одна ячейка свободна, то бот ходит туда
					{
						botstav = 2 + 3 * i;
						cdat = false;
					}
					if (player.tik[2 + 3 * i] && player.tik[1 + 3 * i] && !svob[3 * i]) // если 2 х стоят по горизонтали и одна ячейка свободна, то бот ходит туда (тут в другую сторону)
					{
						botstav = 3 * i;
						cdat = false;
					}
				}				// тут свободные ячейки по диагонали
				if (player.tik[0] && player.tik[4] && !svob[8]) 
				{
					botstav = 8;
					cdat = false;
				}
				if (player.tik[8] && player.tik[4] && !svob[0])  
				{
					botstav = 0;
					cdat = false;
				}
				if (player.tik[2] && player.tik[4] && !svob[6]) 
				{
					botstav = 6;
					cdat = false;
				}
				if (player.tik[6] && player.tik[4] && !svob[2]) 
				{
					botstav = 2;
					cdat = false;
				}
				if (player.tik[2] && player.tik[6] && !svob[4]) 
				{
					botstav = 4;
					cdat = false;
				}
				if (cdat)
				{
					botstav = BotStav();
				}
			}

			bot.tik[botstav] = true;
			svob[botstav] = true;

			hotbot = false;		// если пустого мета нет или игрок уже выиграл, то запрещаем боту делать ход
		}

		player.update(vib);

		int biv = vib + 1;
		if (biv == 3)
		{
			biv = 1;
		}

		bot.update(biv);

		window.clear(Color::Black);		// Фон созданного окна
		if (vib == 0)
		{
			for (int i = 0; i < 2; i++)
			{
				window.draw(vibor[i]);
			}
		}
		else 
		{
			window.draw(fon);
			for (int i = 0; i < 9; i++)      // Когда игрок выбрал за кого будет играть, то программа отрисовывает поле игры
			{
				if (player.tik[i])
				{
					window.draw(player.sprite[i]);
				}
														// Отрисовывает крестик лиюо нолик за игрока и за бота
				if (bot.tik[i])
				{
					window.draw(bot.sprite[i]);
				}
			}
		}
		if (win)
			window.draw(line);
		window.display();
	}

	return 0;
}