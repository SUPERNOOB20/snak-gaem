def count_number_of_lines(nombre_archivo: str) -> int:      # Donde "nombre_archivo" es de tipo *** IN ***.
    archivo = open(nombre_archivo, "r")
    cantidad_lineas: int = 0
    for linea in archivo:
        cantidad_lineas += 1
    return cantidad_lineas

def count_number_of_non_empty_lines(nombre_archivo: str) -> int:      # Donde "nombre_archivo" es de tipo *** IN ***.
    archivo = open(nombre_archivo, "r")
    cantidad_lineas: int = 0
    for linea in archivo:
        if linea != "\n":
            cantidad_lineas += 1
    return cantidad_lineas


def show_bracket_count(nombre_archivo: str):      # Donde "nombre_archivo" es de tipo *** IN ***.
    archivo = open(nombre_archivo, "r")
    number_of_opening_brackets: int = 0
    number_of_closing_brackets: int = 0
    number_of_opening_curly_brackets: int = 0
    number_of_closing_curly_brackets: int = 0
    number_of_opening_square_brackets: int = 0
    number_of_closing_square_brackets: int = 0

    for linea in archivo:
        for caracter in linea:
            if caracter == "(":
                number_of_opening_brackets += 1
            elif caracter == ")":
                number_of_closing_brackets += 1
            elif caracter == "{":
                number_of_opening_curly_brackets += 1
            elif caracter == "}":
                number_of_closing_curly_brackets += 1
            elif caracter == "[":
                number_of_opening_square_brackets += 1
            elif caracter == "]":
                number_of_closing_square_brackets += 1
    
    print("number of ( ---> ", number_of_opening_brackets)
    print("number of ) ---> ", number_of_closing_brackets)
    print("number of { ---> ", number_of_opening_curly_brackets)
    print("number of } ---> ", number_of_closing_curly_brackets)
    print("number of [ ---> ", number_of_opening_square_brackets)
    print("number of ] ---> ", number_of_closing_square_brackets)

    if (number_of_opening_brackets == number_of_closing_brackets) and (number_of_opening_curly_brackets == number_of_closing_curly_brackets) and (number_of_opening_square_brackets == number_of_closing_square_brackets):
        print("Your code's brackets are balanced :3")
    else:
        print("Your code's brackets are NOT balanced x.x")
    return




# res = count_number_of_non_empty_lines("D:/Personal stuff/Peethon/snak_gaem.cpp")
# print(res)

# res = show_bracket_count("D:/Personal stuff/Peethon/snak_gaem.cpp")
##### res = show_bracket_count("./snak_gaem.cpp")
##### res = show_bracket_count("./snak_gaem.h")

res1 = count_number_of_non_empty_lines("./snak_gaem.h")
res2 = count_number_of_non_empty_lines("./snak_gaem.cpp")

print(res1 + res2)
