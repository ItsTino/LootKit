<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

class BuilderController extends Controller
{
    public function showBuilder()
    {
        return view('builder');
    }
}
