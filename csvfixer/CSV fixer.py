import csv
import tkinter as tk

from os.path import isfile
from threading import Thread
from tkinter import filedialog


# https://stackoverflow.com/a/48971137
def bonifica(infile: str, outfile: str):
    try:
        incsv = open(infile, 'r', encoding='utf-8')
        outcsv = open(outfile, 'w', encoding='utf-8', newline='')
        reader = csv.reader(incsv, delimiter=';', quotechar='"')
        writer = csv.writer(outcsv, delimiter=';', quotechar='"')
        reconstructed_row = next(reader)
        length = len(reconstructed_row)
        writer.writerow(list(reconstructed_row))
        reconstructed_row = []
        for row in reader:
            if len(row) < length:
                if reconstructed_row:
                    reconstructed_row[-1] += row[0]
                    reconstructed_row += row[1:]
                else:
                    reconstructed_row = row
            else:
                reconstructed_row = row
            if len(reconstructed_row) == length:
                reconstructed_row = map(lambda s: s.replace('\n', ' '), reconstructed_row)
                writer.writerow(list(reconstructed_row))
                reconstructed_row = []
        tk.Label(text="Elaborazione conclusa con successo").pack()
    except Exception as e:
        tk.Label(text=f"Errore durante l'elaborazione dei dati: {e}").pack()
    finally:
        if incsv != None:
            incsv.close()
        if outcsv != None:
            outcsv.close()


def main():
    window = tk.Tk()
    window.title("CSV fixer")
    
    infile = filedialog.askopenfilename(filetypes=[("File CSV", "*.csv"), ("Tutti i file", "*.*")])
    tk.Label(text=f"File sorgente: {infile}").pack()
    
    outfile = filedialog.asksaveasfilename(defaultextension=".csv", filetypes=[("File CSV", "*.csv")])
    tk.Label(text=f"File destinazione: {outfile}").pack()

    if isfile(infile) and outfile != "":
        tk.Label(text="Elaborazione in corso...").pack()
        t = Thread(target=lambda: bonifica(infile, outfile))
        t.start()
    else:
        tk.Label(text="File sorgente o destinazione non valido").pack()

    window.mainloop()



if __name__ == "__main__":
    main()
