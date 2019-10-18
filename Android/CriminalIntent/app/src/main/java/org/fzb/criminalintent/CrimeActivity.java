package org.fzb.criminalintent;

import androidx.fragment.app.Fragment;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

import java.util.UUID;

public class CrimeActivity extends SingleFragmentActivity implements CrimeFragment.OnFragmentInteractionListener {

    private static final String EXTRA_CRIME_ID = "org.fzb.criminalintent.crime_id";

    @Override
    public void onFragmentInteraction(Uri uri) {

    }

    @Override
    protected Fragment createFragment() {
        UUID crimeId = (UUID) getIntent().getSerializableExtra(EXTRA_CRIME_ID);
        return CrimeFragment.newInstance(crimeId);
    }

    public static Intent newIntent(Context packageContext, UUID crimeId){
        Intent intent = new Intent(packageContext, CrimeActivity.class);
        intent.putExtra(EXTRA_CRIME_ID, crimeId);
        return intent;
    }
}
