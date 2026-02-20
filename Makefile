# ==========================================
# CastOut-Engine Makefile (Fixed & Optimized)
# ==========================================

TARGET = CastOut-Engine

# Компилятор и флаги
CXX = g++
# -MMD генерирует .d файлы для отслеживания изменений в хедерах (.hpp)
CXXFLAGS = -g -Wall -Wextra -std=c++17 -I./headers -I./engine -I./game -MMD
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lm

# Папки
SRC_DIR = src
OBJ_DIR = obj

# Поиск всех .cpp файлов рекурсивно
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")

# Формируем пути для объектных файлов (src/main.cpp -> obj/main.o)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# Цвета для вывода в консоль
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# Основная цель: просто набери 'make', чтобы собрать проект
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	@echo "$(GREEN)🔗 Связывание объектов в $(TARGET)...$(NC)"
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "$(GREEN)✅ Готово!$(NC)"

# Компиляция .cpp в .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)🔨 Компиляция $<...$(NC)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Запуск приложения (теперь run ЗАВИСИТ от $(TARGET))
# Если ты изменил код, make сначала пересоберет его, а потом запустит
run: $(TARGET)
	@echo "$(GREEN)🚀 Запуск $(TARGET)...$(NC)"
	@./$(TARGET) || echo "$(YELLOW)⚠️  Приложение завершилось. Посмотри ошибки выше.$(NC)"

# Очистка проекта
clean:
	@echo "🧹 Очистка папки $(OBJ_DIR) и файла $(TARGET)..."
	rm -rf $(OBJ_DIR) $(TARGET)

# Подключение файлов зависимостей (чтобы hpp изменения тоже подхватывались)
-include $(DEPS)

.PHONY: all run clean