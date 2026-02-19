RM = rm -rfv

SRC +=		\
main.c		\
print.c		\
graphique.c
# resolveur_EDO.c

OBJ +=		\
obj/main.o	\
obj/print.o	\
obj/graphique.o
# obj/resolveur_EDO.o

OUTPUT_FILE =	\
obj/print_console.elf

HEX =	\
print_console.hex

FLAGS +=	\
-x c -funsigned-char -funsigned-bitfields -DDEBUG

FLAGS_CLASSIQUE +=							\
-Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums	\
-g2 -Wall -mmcu=atmega328p -B atmega328p -c -std=gnu99


./obj/%.o: ./%.c
	@echo
	@echo Building: $<
	# avr-gcc -Os -g -mmcu=atmega328p -I/usr/lib/avr ./$(SRC)
	avr-gcc $(FLAGS) -I/usr/lib/avr $(FLAGS_CLASSIQUE) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -o "$@" "$<"
	@echo Finished_building: $<


all: $(OUTPUT_FILE)

$(OUTPUT_FILE): $(OBJ)
	@echo
	@echo Building_target: $@
	avr-gcc -o$(OUTPUT_FILE) $(OBJ) -Wl,-Map="obj/USART.map" -Wl,--start-group -Wl,-lm -Wl,-lprintf_flt  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p -B atmega328p -Wl,-u,vfprintf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "obj/print_console.elf" "print_console.hex"
	@echo Finished_building_target: $@
	@echo
	# avr-objcopy -O ihex -R .eeprom a.out $(HEX)
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "obj/print_console.elf" "obj/print_console.hex"

spec:
	avr-objcopy -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "obj/print_console.elf" "obj/print_console.eep" || exit 0
	avr-objdump -h -S "obj/print_console.elf" > "obj/print_console.lss"
	avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "obj/print_console.elf" "obj/print_console.srec"
	avr-size "obj/print_console.elf"


dude:
	avrdude -C /etc/avrdude.conf -c arduino -p atmega328p -D wiring -P /dev/ttyACM0 -b 115200 -U flash:w:$(HEX):a


clean:
	$(RM) $(HEX)
	$(RM) obj/*
	$(RM) $(OUTPUT_FILE)
