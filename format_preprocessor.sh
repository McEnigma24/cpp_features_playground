#!/bin/bash

# Skrypt do formatowania tylko linii z dyrektywami preprocesora
# Użycie: ./format_preprocessor.sh plik.cpp

if [ $# -ne 1 ]; then
    echo "Użycie: $0 <plik>"
    exit 1
fi

FILE="$1"
TEMP_FILE=$(mktemp)
TEMP_PREPROC=$(mktemp)

# Wyodrębnij linie zaczynające się od # (z wcięciami)
grep -n "^[[:space:]]*#" "$FILE" > "$TEMP_PREPROC"

if [ ! -s "$TEMP_PREPROC" ]; then
    echo "Brak linii z dyrektywami preprocesora w pliku $FILE"
    rm -f "$TEMP_FILE" "$TEMP_PREPROC"
    exit 0
fi

# Stwórz plik tymczasowy z tylko liniami preprocesora
while IFS=: read -r line_num content; do
    echo "$content" >> "$TEMP_FILE"
done < "$TEMP_PREPROC"

# Uruchom indent na liniach preprocesora
indent -kr -i4 -ts4 -l80 -ss -sob "$TEMP_FILE" 2>/dev/null

if [ $? -ne 0 ]; then
    echo "Błąd podczas formatowania linii preprocesora"
    rm -f "$TEMP_FILE" "$TEMP_PREPROC"
    exit 1
fi

# Wczytaj sformatowane linie do tablicy
mapfile -t formatted_lines < "$TEMP_FILE"

# Zastąp oryginalne linie sformatowanymi
formatted_index=0
while IFS= read -r line; do
    if echo "$line" | grep -q "^[[:space:]]*#"; then
        # Użyj sformatowanej linii
        echo "${formatted_lines[$formatted_index]}"
        formatted_index=$((formatted_index + 1))
    else
        # Zachowaj oryginalną linię
        echo "$line"
    fi
done < "$FILE" > "${FILE}.tmp"

mv "${FILE}.tmp" "$FILE"

echo "Sformatowano linie preprocesora w pliku $FILE"

# Wyczyść pliki tymczasowe
rm -f "$TEMP_FILE" "$TEMP_PREPROC"
