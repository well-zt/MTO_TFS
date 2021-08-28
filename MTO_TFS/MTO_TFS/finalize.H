
VecDestroy(&xmma);
VecDestroy(&xold);
VecDestroy(&xmin);
VecDestroy(&xmax);
VecDestroy(&dfdx);
VecDestroyVecs(m, &dgdx);
delete[] gx;
delete mma;

Info<< " \n" << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
<< nl << endl;