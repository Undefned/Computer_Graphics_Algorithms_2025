from PIL import Image
import numpy as np

def image_to_c_array(image_path, output_file):
    # Загружаем изображение
    img = Image.open(image_path)
    
    # Конвертируем в RGB если нужно
    if img.mode != 'RGB':
        img = img.convert('RGB')
    
    # Получаем данные
    data = np.array(img)
    
    # Записываем в файл
    with open(output_file, 'w') as f:
        f.write("/* Generated from {} */\n".format(image_path))
        f.write("static const unsigned char\n")
        f.write("myDemonTextureImage[] = {\n")
        
        # Пишем данные в формате RGB
        rows, cols = data.shape[:2]
        for i in range(rows):
            row_data = data[i]
            line = ""
            for j in range(cols):
                r, g, b = row_data[j]
                line += " {}, {}, {},".format(r, g, b)
                # Перенос строки каждые 6 пикселей
                if (i * cols + j + 1) % 6 == 0:
                    line += "\n"
            f.write(line)
        
        f.write("\n};\n")
    
    print(f"Создан файл: {output_file}")

# Использование
image_to_c_array("you_are_my_sunshine.png", "my_image.h")