package org.fzb.criminalintent;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

import org.fzb.criminalintent.database.StudentDbSchema.CrimeBaseHelper;
import org.fzb.criminalintent.database.StudentDbSchema.CrimeCursorWrapper;
import org.fzb.criminalintent.database.StudentDbSchema.CrimeDbSchema.CrimeTable;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class CrimeLab {
    private static CrimeLab sCrimeLab;
    //private List<Crime> mCrimes;
    //private LinkedHashMap<UUID, Crime> mCrimes;
    private Context mContext;
    private SQLiteDatabase mDatabase;

    public static CrimeLab get(Context context) {
        if (sCrimeLab == null) {
            sCrimeLab = new CrimeLab(context);
        }

        return sCrimeLab;
    }

    private CrimeLab(Context context) {
        mContext = context.getApplicationContext();
        mDatabase = new CrimeBaseHelper(mContext).getWritableDatabase();
        //mCrimes = new ArrayList<>();
        //mCrimes = new LinkedHashMap<>();
//        for(int i=0; i<100; i++){
//            Crime crime = new Crime();
//            crime.setTitle("Crime #" + i);
//            crime.setSolved(i % 2 == 0);
//            //mCrimes.add(crime);
//            mCrimes.put(crime.getId(), crime);
//        }
    }

    public List<Crime> getCrimes() {
        //return mCrimes;
        //return new ArrayList<>(mCrimes.values());
        List<Crime> crimes = new ArrayList<>();

        CrimeCursorWrapper cursor = querCrimes(null, null);

        try {
            cursor.moveToFirst();
            while (!cursor.isAfterLast()) {
                crimes.add(cursor.getCrime());
                cursor.moveToNext();
            }
        } finally {
            cursor.close();
        }

        return crimes;
    }

    public Crime getCrime(UUID id) {
//        for(Crime crime : mCrimes){
////            if(crime.getId().equals(id)){
////                return crime;
////            }
////        }
////        return null;

        //return mCrimes.get(id);

        CrimeCursorWrapper cursor = querCrimes(
                CrimeTable.Cols.UUID + " = ?",
                new String[]{id.toString()}
        );

        try {
            if (cursor.getCount() == 0) {
                return null;
            }

            cursor.moveToFirst();
            return cursor.getCrime();
        } finally {
            cursor.close();
        }
    }

    public void addCrime(Crime crime) {
        //mCrimes.put(crime.getId(), crime);
        ContentValues values = getContentValues(crime);
        try {
            mDatabase.insert(CrimeTable.NAME, null, values);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void updateCrime(Crime crime) {
        String uuidString = crime.getId().toString();
        ContentValues values = getContentValues(crime);
        try {
            mDatabase.update(CrimeTable.NAME, values,
                    CrimeTable.Cols.UUID + " = ?",
                    new String[]{uuidString});
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public void removeCrime(Crime crime) {
        //mCrimes.remove(crime.getId());
        String uuidString = crime.getId().toString();
        mDatabase.delete(
                CrimeTable.NAME,
                CrimeTable.Cols.UUID + " = ?",
                new String[]{uuidString});
    }

    public void deleteCrime(Crime crime) {
        String uuidString = crime.getId().toString();

        try {
            mDatabase.delete(
                    CrimeTable.NAME,
                    CrimeTable.Cols.UUID + " = ?",
                    new String[]{uuidString});
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private CrimeCursorWrapper querCrimes(String whereClause, String[] whereArgs) {
        Cursor cursor = null;
        try {
            cursor = mDatabase.query(
                    CrimeTable.NAME,
                    null,
                    whereClause,
                    whereArgs,
                    null,
                    null,
                    null
            );
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new CrimeCursorWrapper(cursor);
    }

    private static ContentValues getContentValues(Crime crime) {
        ContentValues values = new ContentValues();
        values.put(CrimeTable.Cols.UUID, crime.getId().toString());
        values.put(CrimeTable.Cols.TITLE, crime.getTitle());
        values.put(CrimeTable.Cols.DATE, crime.getDate().getTime());
        values.put(CrimeTable.Cols.SOLVED, crime.isSolved() ? 1 : 0);
        values.put(CrimeTable.Cols.SUSPECT, crime.getSuspect());

        return values;
    }
}
