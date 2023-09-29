<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Hash;

class LoginController extends Controller
{
    public function showLoginForm()
    {
        return view('login');
    }

    public function login(Request $request)
    {
        $inputPassword = $request->input('password');
        $hashedPassword = config('app.admin_password');

        if (Hash::check($inputPassword, $hashedPassword)) {
            session(['authenticated' => true]);
            return redirect()->intended('/dashboard');
        }

        return back()->withErrors(['password' => 'The provided password is incorrect.']);
    }
}
