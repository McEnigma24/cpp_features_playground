#!/usr/bin/env python3

import re
import sys
import os

def format_preprocessor_directives(file_path):
    """Formatuje dyrektywy preprocesora wyrównując je do poziomu zagnieżdżenia"""
    
    # Utwórz nazwę pliku wyjściowego z sufiksem _changed
    base_name, ext = os.path.splitext(file_path)
    output_file = f"{base_name}_changed{ext}"
    
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    
    formatted_lines = []
    indent_level = 0  # Poziom zagnieżdżenia (liczba wcięć po 4 spacje)
    in_preprocessor_block = False
    preprocessor_block_started = False
    
    for i, line in enumerate(lines):
        stripped_line = line.strip()
        
        # Sprawdź czy linia zaczyna się od # (z wcięciami)
        if re.match(r'^\s*#', line):
            content = stripped_line
            
            # Określ wcięcie na podstawie aktualnego poziomu zagnieżdżenia
            target_indent = "    " * indent_level
            
            # Sprawdź czy to początek bloku preprocesora
            if content.startswith('#if') or content.startswith('#ifdef') or content.startswith('#ifndef'):
                in_preprocessor_block = True
                preprocessor_block_started = True
                # #if, #ifdef, #ifndef - na poziomie aktualnego zagnieżdżenia
                formatted_line = f"{target_indent}{content}"
                formatted_lines.append(formatted_line + '\n')
            elif content.startswith('#endif'):
                # #endif - na poziomie aktualnego zagnieżdżenia
                formatted_line = f"{target_indent}{content}"
                formatted_lines.append(formatted_line + '\n')
                in_preprocessor_block = False
                preprocessor_block_started = False
            elif content.startswith('#elif') or content.startswith('#else'):
                # #elif, #else - na poziomie aktualnego zagnieżdżenia
                formatted_line = f"{target_indent}{content}"
                formatted_lines.append(formatted_line + '\n')
            elif content.startswith('#define') and in_preprocessor_block:
                # #define wewnątrz bloku preprocesora - dodatkowe wcięcie
                target_indent_define = target_indent + "    "
                formatted_line = f"{target_indent_define}{content}"
                formatted_lines.append(formatted_line + '\n')
            elif content.startswith('#define'):
                # #define poza blokiem - na poziomie aktualnego zagnieżdżenia
                formatted_line = f"{target_indent}{content}"
                formatted_lines.append(formatted_line + '\n')
            else:
                # Inne dyrektywy - na poziomie aktualnego zagnieżdżenia
                formatted_line = f"{target_indent}{content}"
                formatted_lines.append(formatted_line + '\n')
        else:
            # Linie bez # - sprawdź czy są wewnątrz bloku preprocesora
            if in_preprocessor_block and preprocessor_block_started and stripped_line:
                # Sprawdź czy to nawiasy klamrowe
                if stripped_line == '{' or stripped_line == '}':
                    # Nawiasy klamrowe - na tym samym poziomie co #if/#elif
                    target_indent = "    " * indent_level
                    # Sprawdź czy już ma odpowiednie wcięcie
                    current_indent = "    " * indent_level
                    if line.startswith(current_indent):
                        # Już ma odpowiednie wcięcie - zostaw bez zmian
                        formatted_lines.append(line)
                    else:
                        # Nie ma odpowiedniego wcięcia - dodaj
                        formatted_line = f"{target_indent}{stripped_line}"
                        formatted_lines.append(formatted_line + '\n')
                else:
                    # Linie kodu wewnątrz bloku preprocesora - dodatkowe wcięcie
                    target_indent = "    " * (indent_level + 1)
                    # Sprawdź czy już ma odpowiednie wcięcie
                    if line.startswith(target_indent):
                        # Już ma odpowiednie wcięcie - zostaw bez zmian
                        formatted_lines.append(line)
                    else:
                        # Nie ma odpowiedniego wcięcia - dodaj
                        formatted_line = f"{target_indent}{stripped_line}"
                        formatted_lines.append(formatted_line + '\n')
            else:
                # Linie poza blokiem preprocesora - zachowaj bez zmian
                formatted_lines.append(line)
            
            # Śledź poziom zagnieżdżenia na podstawie nawiasów klamrowych
            for char in line:
                if char == '{':
                    indent_level += 1
                elif char == '}':
                    indent_level = max(0, indent_level - 1)
    
    # Zapisz sformatowane linie do nowego pliku
    with open(output_file, 'w', encoding='utf-8') as file:
        file.writelines(formatted_lines)
    
    print(f"Sformatowano dyrektywy preprocesora w pliku: {file_path}")
    print(f"Wynik zapisano jako: {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Użycie: python format_preprocessor_v3.py <plik>")
        sys.exit(1)
    
    format_preprocessor_directives(sys.argv[1])
