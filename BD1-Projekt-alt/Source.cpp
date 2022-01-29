#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

//Usuwanie tabel
void DB_delete(sql::Statement *stmt, sql::Connection *con) {
    stmt->execute("DROP TABLE IF EXISTS Szpitale");
    stmt->execute("DROP TABLE IF EXISTS Garaze");
    stmt->execute("DROP TABLE IF EXISTS Pojazdy");
    stmt->execute("DROP TABLE IF EXISTS Helikoptery");
    stmt->execute("DROP TABLE IF EXISTS Karetki");
    stmt->execute("DROP TABLE IF EXISTS Ratownicy");
    stmt->execute("DROP TABLE IF EXISTS Kierowcy");
    stmt->execute("DROP TABLE IF EXISTS Uszkodzenia");
    stmt->execute("DROP TABLE IF EXISTS Wyposazenie");
    stmt->execute("DROP TABLE IF EXISTS Akcje");
    stmt->execute("DROP TABLE IF EXISTS Pacjenci");
    stmt->execute("DROP TABLE IF EXISTS Wezwania");
    stmt->execute("DROP TABLE IF EXISTS Operatorzy");
}

//Tworzenie tabel
void DB_create(sql::Statement* stmt, sql::Connection* con) {
    stmt->execute("CREATE TABLE Operatorzy("
        "Operator_ID VARCHAR(4) PRIMARY KEY,"
        "Imie VARCHAR(32),"
        "Nazwisko VARCHAR(32),"
        "PESEL VARCHAR(17),"
        "Pensja NUMERIC)");

    stmt->execute("CREATE TABLE Wezwania("
        "Wezwanie_ID VARCHAR(4) PRIMARY KEY,"
        "Operator_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Numer_wzywaj¹cego VARCHAR(16),"
        "Powod VARCHAR(32),"
        "Miasto_wezwania VARCHAR(16),"
        "Liczbarannych NUMERIC,"
        "CONSTRAINT wezw_oper_fk FOREIGN KEY(Operator_ID) REFERENCES Operatorzy(Operator_ID))");

    stmt->execute("CREATE TABLE Pacjenci("
        "Pacjent_ID VARCHAR(4) PRIMARY KEY,"
        "Imie VARCHAR(32),"
        "Nazwisko VARCHAR(32),"
        "PESEL VARCHAR(17),"
        "Uraz VARCHAR(64),"
        "Grupa_krwi VARCHAR(3))");

    stmt->execute("CREATE TABLE Akcje("
        "Akcja_ID VARCHAR(4) PRIMARY KEY,"
        "Pacjent_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Wezwanie_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Pojazd_ID VARCHAR(4) NOT NULL UNIQUE,"
        "CONSTRAINT akc_pacj_fk FOREIGN KEY(Pacjent_ID) REFERENCES Pacjenci(Pacjent_ID),"
        "CONSTRAINT akc_wezw_fk FOREIGN KEY(Wezwanie_ID) REFERENCES Wezwania(Wezwanie_ID))");

    stmt->execute("CREATE TABLE Wyposazenie("
        "Wyposazenie_ID VARCHAR(4) PRIMARY KEY," 
        "Zastosowanie VARCHAR(32),"
        "Nazwa VARCHAR(32))");

    stmt->execute("CREATE TABLE Uszkodzenia("
        "Uszkodzenie_ID VARCHAR(4) PRIMARY KEY,"
        "Sprawca_uszkodzenia VARCHAR(32),"
        "Rodzaj_uszkodzenia VARCHAR(32),"
        "Koszt NUMERIC)");

    stmt->execute("CREATE TABLE Kierowcy("
        "Kierowca_ID VARCHAR(4) PRIMARY KEY,"
        "Imie VARCHAR(32),"
        "Nazwisko VARCHAR(32),"
        "PESEL VARCHAR(17))");

    stmt->execute("CREATE TABLE Ratownicy("
        "Ratownik_ID VARCHAR(4) PRIMARY KEY,"
        "Imie VARCHAR(32),"
        "Nazwisko VARCHAR(32),"
        "PESEL VARCHAR(17),"
        "Pensja NUMERIC)");

    stmt->execute("CREATE TABLE Karetki("
        "Numer_VIN VARCHAR(17) PRIMARY KEY,"
        "Numer_rejestracyjny VARCHAR(8) NOT NULL UNIQUE,"
        "Zasieg NUMERIC)");

    stmt->execute("CREATE TABLE Helikoptery("
        "Helikopter_ID VARCHAR(4) PRIMARY KEY,"
        "Numer_rejestracyjny VARCHAR(8) NOT NULL UNIQUE,"
        "Zasieg NUMERIC)");

    stmt->execute("CREATE TABLE Pojazdy("
        "Pojazd_ID VARCHAR(4) PRIMARY KEY,"
        "Wyposazenie_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Uszkodzenie_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Kierowca_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Ratownik_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Numer_VIN VARCHAR(17) UNIQUE,"
        "Numer_rejestracyjny VARCHAR(8) UNIQUE,"
        "CONSTRAINT poj_akc_fk FOREIGN KEY(Pojazd_ID) REFERENCES Akcje(Pojazd_ID),"
        "CONSTRAINT poj_wyp_fk FOREIGN KEY(Wyposazenie_ID) REFERENCES Wyposazenie(Wyposazenie_ID),"
        "CONSTRAINT poj_uszk_fk FOREIGN KEY(Uszkodzenie_ID) REFERENCES Uszkodzenia(Uszkodzenie_ID),"
        "CONSTRAINT poj_kier_fk FOREIGN KEY(Kierowca_ID) REFERENCES Kierowcy(Kierowca_ID),"
        "CONSTRAINT poj_rat_fk FOREIGN KEY(Ratownik_ID) REFERENCES Ratownicy(Ratownik_ID),"
        "CONSTRAINT poj_kar_fk FOREIGN KEY(Numer_VIN) REFERENCES Karetki(Numer_Vin),"
        "CONSTRAINT poj_hel_fk FOREIGN KEY(Numer_rejestracyjny) REFERENCES Helikoptery(Numer_rejestracyjny))");

    stmt->execute("CREATE TABLE Garaze("
        "Garaz_ID VARCHAR(4) PRIMARY KEY,"
        "Liczba_miejsc NUMERIC,"
        "Numer_VIN VARCHAR(17) UNIQUE,"
        "Helikopter_ID VARCHAR(4) UNIQUE,"
        "CONSTRAINT gar_kar_fk FOREIGN KEY(Numer_VIN) REFERENCES Karetki(Numer_VIN),"
        "CONSTRAINT gar_hel_fk FOREIGN KEY(Helikopter_ID) REFERENCES Helikoptery(Helikopter_ID))");

    stmt->execute("CREATE TABLE Szpitale("
        "Szpital_ID VARCHAR(4) PRIMARY KEY,"
        "Garaz_ID VARCHAR(4) NOT NULL UNIQUE,"
        "Nazwa VARCHAR(64),"
        "Adres VARCHAR(32),"
        "Numer_telefonu VARCHAR(11),"
        "Zarzadca VARCHAR(32),"
        "CONSTRAINT szp_gar_fk FOREIGN KEY(Garaz_ID) REFERENCES Garaze(Garaz_ID))");
}

//Inserty
void DB_inserts(sql::Statement* stmt, sql::Connection* con) {
    
    stmt->execute("INSERT INTO Operatorzy VALUES('1', 'Mateusz', 'Krzysiek', 69420213790, 2137)");
    stmt->execute("INSERT INTO Operatorzy VALUES('2', 'Kamil', 'Zdun', 69420213791, 1337)");
    stmt->execute("INSERT INTO Operatorzy VALUES('3', 'Joanna', 'Slawojka', 69420213792, 3547)");

    stmt->execute("INSERT INTO Wezwania VALUES('1','1','123456789','Upadek','Zakopane', 1)");
    stmt->execute("INSERT INTO Wezwania VALUES('2','2','123456790','Zawal serca','Warszawa', 1)");

    stmt->execute("INSERT INTO Pacjenci VALUES('1','Michal','Parchas','70420213790','Upadek', 'A-')");
    stmt->execute("INSERT INTO Pacjenci VALUES('2','Michal','Jachas','70420213791','Zawal', 'B+')");
    
    stmt->execute("INSERT INTO Akcje VALUES('1', '1', '1', '1')");
    stmt->execute("INSERT INTO Akcje VALUES('2', '2', '2', '2')");

    stmt->execute("INSERT INTO Wyposazenie VALUES('1', 'Stablizacja konczyn', 'Szyna stabilizujaca')");
    stmt->execute("INSERT INTO Wyposazenie VALUES('2', 'Stabilizacja pracy serca', 'Defibrylator')");

    stmt->execute("INSERT INTO Uszkodzenia VALUES('1', 'Pacjent', 'Nadgryzione_lozko', 300)");
    stmt->execute("INSERT INTO Uszkodzenia VALUES('2', 'Przechodzien', 'Zarysowana karoseria', 500)");

    stmt->execute("INSERT INTO Kierowcy VALUES('1', 'Robert', 'Kica', '71420213790')");
    stmt->execute("INSERT INTO Kierowcy VALUES('2', 'Janusz', 'Korkociag', '71420213791')");

    stmt->execute("INSERT INTO Ratownicy VALUES('1', 'Artur', 'Kaminski', '72420213790', 4096)");
    stmt->execute("INSERT INTO Ratownicy VALUES('2', 'Dominik', 'Zielony', '72420213791', 3156)");

    stmt->execute("INSERT INTO Karetki VALUES('AAAZZZ7C56D122334', 'VAU1245C', 500)");
    stmt->execute("INSERT INTO Karetki VALUES('PEUIKS7C56D122335', 'WEU1246B', 600)");

    stmt->execute("INSERT INTO Helikoptery VALUES('0', 'HEL0', 0)");//Helikopter widmo dla po³¹czenia z innymi tabelami
    stmt->execute("INSERT INTO Helikoptery VALUES('1', 'HEL1', 900)");
    stmt->execute("INSERT INTO Helikoptery VALUES('2', 'HEL2', 1200)");
    
    stmt->execute("INSERT INTO Pojazdy VALUES('1', '1', '1', '1', '1', 'AAAZZZ7C56D122334', 'HEL0')");//karetki
    stmt->execute("INSERT INTO Pojazdy VALUES('2', '2', '2', '2', '2', NULL, 'HEL1')");//Helikoptery

    stmt->execute("INSERT INTO Garaze VALUES('1', 10, 'AAAZZZ7C56D122334', '0')");//Karetki
    stmt->execute("INSERT INTO Garaze VALUES('2', 5, NULL, '2')");//Helikoptery

    stmt->execute("INSERT INTO Szpitale VALUES('1', '1', 'Szpital Astralny', 'Radom 1', '122123326', 'ZUS')");
    stmt->execute("INSERT INTO Szpitale VALUES('2', '2', 'Szpital Niepokalanego dzieciatka Judasz', 'Bydgoszcz 14', '122123327', 'Miasto Bydgoszcz')");

}

int main(void)
{
    try {
        sql::Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;

        //Pod³aczenie do serwera
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "qwerty");
        //Pod³¹czenie do BD
        con->setSchema("Pogotowie");

        int input = 0, view_selector = 0;;

        stmt = con->createStatement();
        while (input > -1) {
            cout << "Baza danych Pogotowia Ratunkowego." << endl << "Operacje:" << endl;
            cout << "1.Inicjalizacja bazy" << endl;
            cout << "2.Utworzenie Bazy" << endl;
            cout << "3.Dodanie podstawowych danych." << endl;
            cout << "4.Usuniecie bazy." << endl;
            cout << "5.Widoki" << endl;
            cin >> input;

            switch (input) {
            case 1:
                DB_delete(stmt, con);
                DB_create(stmt, con);
                DB_inserts(stmt, con);
                break;
            case 2:
                DB_create(stmt, con);
                break;
            case 3:
                DB_inserts(stmt, con);
                break;
            case 4:
                DB_delete(stmt, con);
                break;
            case 5:
                while (view_selector > -1) {
                    cout << "Widoki" << endl;
                    cout << "1.Pacjenci i Szpitale" << endl;
                    cout << "2.Karetki i uszkodzenia" << endl;
                    cout << "3." << endl;
                    cin >> view_selector;
                
                    switch (view_selector) {
                    case 1:
                        pstmt = con->prepareStatement("SELECT Pacjenci.Pacjent_ID, Pacjenci.Imie,"
                            "Pacjenci.Nazwisko, Pacjenci.PESEL, Szpitale.Nazwa, Szpitale.Adres,"
                            "Szpitale.Numer_telefonu FROM Pacjenci JOIN Akcje ON Pacjenci.Pacjent_ID = Akcje.pacjent_ID JOIN Pojazdy ON Pojazdy.Pojazd_ID = Akcje.Pojazd_ID JOIN Karetki ON Karetki.Numer_VIN = Pojazdy.Numer_VIN JOIN Garaze ON Garaze.Numer_VIN = Karetki.Numer_VIN JOIN Szpitale ON Garaze.Garaz_ID = Szpitale.Garaz_ID");
                        res = pstmt->executeQuery();
                        res->beforeFirst();
                        while (res->next())
                            cout << res->getString("Imie") << ' ' << res->getString("Nazwisko") << ' ' << res->getString("Nazwa") << endl;
                        break;
                    case 2:
                        pstmt = con->prepareStatement("SELECT Uszkodzenia.Rodzaj_uszkodzenia, Karetki.Numer_VIN, Karetki.Numer_rejestracyjny,"
                            "Karetki.Zasieg FROM Uszkodzenia JOIN Pojazdy ON Pojazdy.Uszkodzenie_ID = Uszkodzenia.Uszkodzenie_ID JOIN Karetki ON Karetki.Numer_VIN = Pojazdy.Numer_VIN");
                        res = pstmt->executeQuery();
                        res->beforeFirst();
                        while (res->next())
                            cout << res->getString("Rodzaj_uszkodzenia") << ' ' << res->getString("Numer_VIN") << ' ' << res->getString("Numer_rejestracyjny") << res->getString("Zasieg") << ' ' << endl;
                        break;
                    case 3:
                        pstmt = con->prepareStatement("SELECT Operatorzy.Imie,"
                            "Wezwania.Miasto_wezwania FROM Operatorzy JOIN Wezwania ON Operatorzy.Operator_ID = Wezwania.Operator_ID");
                        res = pstmt->executeQuery();
                        res->beforeFirst();
                        while (res->next())
                            cout << res->getString("Imie") << ' ' << res->getString("Miasto_wezwania") << endl;
                        break;
                    default:
                        return 0;
                        break;
                    }
                }
            default:
                return 0;
                break;
            }
        }
        delete stmt;

        
        pstmt = con->prepareStatement("SELECT * FROM Wezwania");
        res = pstmt->executeQuery();
        res->beforeFirst();
        while (res->next())
            cout << res->getString("Miasto_wezwania") << endl;



        delete res;
        delete pstmt;
        delete con;
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;

    return EXIT_SUCCESS;
}